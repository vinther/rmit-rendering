/*
 * Octree.cpp
 *
 *  Created on: 05/08/2013
 *      Author: svp
 */

#include "physics/Octree.hpp"

#include <assimp/scene.h>
#include <SDL2/SDL_log.h>
#include <glm/gtx/intersect.hpp>

#include "physics/Intersections.hpp"
#include "assets/Model.hpp"
#include "scene/SceneNode.hpp"

#include "Utilities.hpp"

template <unsigned int bucketSize>
physics::OctreeT<bucketSize>::OctreeT()
{
}

template <unsigned int bucketSize>
physics::OctreeT<bucketSize>::~OctreeT()
{
}

template <typename NodeType>
inline bool insertTriangle(
        const physics::Triangle& triangle,
        NodeType& node,
        unsigned int idx,
        const std::vector<physics::Triangle>& objects)
{
    if (!physics::triangleAABBIntersection(triangle, node.aabb))
        return false;

    if (!node.isLeaf)
    {
        for (auto& child: node.children)
        {
            insertTriangle(triangle, *child, idx, objects);
        }
    } else if (node.isLeaf && node.bucket.size() < node.nodeBucketSize)
    {
        node.bucket.push_back(idx);
    } else
    {
        for (auto& child: node.children)
        {
            child = std::make_unique<NodeType>();
        }

        const auto& min = node.aabb.min;
        const auto& max = node.aabb.max;
        const auto half = glm::vec3(0.5f, 0.5f, 0.5f) * (max - min);

        node.children[0]->aabb.min = min + glm::vec3(0.0f, 0.0f, 0.0f) * half;
        node.children[1]->aabb.min = min + glm::vec3(0.0f, 1.0f, 0.0f) * half;
        node.children[2]->aabb.min = min + glm::vec3(1.0f, 1.0f, 0.0f) * half;
        node.children[3]->aabb.min = min + glm::vec3(1.0f, 0.0f, 0.0f) * half;

        node.children[4]->aabb.min = min + glm::vec3(1.0f, 0.0f, 1.0f) * half;
        node.children[5]->aabb.min = min + glm::vec3(1.0f, 1.0f, 1.0f) * half;
        node.children[6]->aabb.min = min + glm::vec3(0.0f, 1.0f, 1.0f) * half;
        node.children[7]->aabb.min = min + glm::vec3(0.0f, 0.0f, 1.0f) * half;

        for (auto& child: node.children)
        {
            child->aabb.max = child->aabb.min + half;

            for (auto triangleIdx: node.bucket)
                insertTriangle(objects[triangleIdx], *child, triangleIdx, objects);

            insertTriangle(triangle, *child, idx, objects);
        }

        node.bucket.empty();
        node.isLeaf = false;
    }

    return true;
}

template <typename NodeType>
void insertFace(
        const aiFace& face,
        const aiVector3D* vertices,
        NodeType& root,
        std::vector<physics::Triangle>& objects,
        unsigned int idx)
{
    physics::Triangle triangle;

    const aiVector3D& v0 = vertices[face.mIndices[0]];
    const aiVector3D& v1 = vertices[face.mIndices[1]];
    const aiVector3D& v2 = vertices[face.mIndices[2]];

    triangle.vertices[0] = glm::vec3(v0.x, v0.y, v0.z);
    triangle.vertices[1] = glm::vec3(v1.x, v1.y, v1.z);
    triangle.vertices[2] = glm::vec3(v2.x, v2.y, v2.z);

    objects[idx] = triangle;

    insertTriangle<NodeType>(triangle, root, idx, objects);
}

template <typename NodeType>
void insertMesh(
        const aiMesh& mesh,
        NodeType& root,
        std::vector<physics::Triangle>& objects,
        unsigned int startIdx)
{
    for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
    {
        insertFace<NodeType>(mesh.mFaces[i], mesh.mVertices, root, objects, startIdx + i);
    }
}

template <unsigned int bucketSize>
void physics::OctreeT<bucketSize>::createFromScene(const scene::SceneNode& sceneRoot)
{
    const float limitMin = std::numeric_limits<float>::min();
    const float limitMax = std::numeric_limits<float>::max();

    glm::vec3 min(limitMax, limitMax, limitMax);
    glm::vec3 max(limitMin, limitMin, limitMin);

    unsigned int totalNumFaces = 0;
    for (unsigned int i = 0; i < sceneRoot.model->scene->mNumMeshes; ++i)
    {
        const auto& mesh = *(sceneRoot.model->scene->mMeshes[i]);

        for (unsigned int j = 0; j < mesh.mNumVertices; ++j)
        {
            const auto& v = mesh.mVertices[j];

            min = glm::vec3(glm::min(min.x, v.x), glm::min(min.y, v.y), glm::min(min.z, v.z));
            max = glm::vec3(glm::max(max.x, v.x), glm::max(max.y, v.y), glm::max(max.z, v.z));
        }

        totalNumFaces += mesh.mNumFaces;
    }

    min = min - glm::vec3(1.0f, 1.0f, 1.0f);
    max = max + glm::vec3(1.0f, 1.0f, 1.0f);

    objects.resize(totalNumFaces);

    root = std::make_unique<Node>();
    root->aabb.min = min;
    root->aabb.max = max;

    unsigned int startIdx = 0;
    for (unsigned int i = 0; i < sceneRoot.model->scene->mNumMeshes; ++i)
    {
        const auto& mesh = *(sceneRoot.model->scene->mMeshes[i]);

        insertMesh(mesh, *root, objects, startIdx);

        startIdx += mesh.mNumFaces;
    }

    optimize();

    root.reset();
    objects.empty();
}

template <typename NodeType, typename BucketType>
unsigned int insertDFS(
        const NodeType& ptrNode,
        const std::vector<physics::Triangle>& objects,

        unsigned int node,

        std::vector<std::array<unsigned int, 8>>& nodeChildren,
        std::vector<std::array<bool, 8>>& nodeChildIsLeaf,
        std::vector<std::array<physics::AABB, 8>>& nodeChildAABB,
        std::vector<BucketType>& bucket)
{
    for (unsigned int i = 0; i < 8; ++i)
    {
        nodeChildIsLeaf[node][i] = ptrNode->children[i]->isLeaf;
        nodeChildAABB[node][i] = ptrNode->children[i]->aabb;

        if (!nodeChildIsLeaf[node][i])
            continue;

        nodeChildren[node][i] = bucket.size();

        bucket.emplace_back();
        bucket[nodeChildren[node][i]].first = ptrNode->children[i]->bucket.size();

        for (unsigned int j = 0; j < ptrNode->children[i]->bucket.size(); ++j)
            bucket[nodeChildren[node][i]].second[j] = objects[ptrNode->children[i]->bucket[j]];
    }

    unsigned int nextIdx = node + 1;
    for (unsigned int i = 0; i < 8; ++i)
    {
        if (!nodeChildIsLeaf[node][i])
        {
            nodeChildren[node][i] = nextIdx;

            nextIdx = insertDFS(ptrNode->children[i], objects, nextIdx, nodeChildren, nodeChildIsLeaf, nodeChildAABB, bucket);
        }
    }

    return nextIdx;
}

template <typename NodeType>
void countNodes(const NodeType& node, unsigned int& internalNodes, unsigned int& leaves)
{
    if (node->isLeaf)
    {
        leaves += 1;
    } else
    {
        internalNodes += 1;

        for (const auto& child: node->children)
        {
            countNodes(child, internalNodes, leaves);
        }
    }
}

inline void getBuckets(
        unsigned int node,
        std::vector<unsigned int>& buckets,

        const physics::Ray& ray,
        const std::vector<std::array<unsigned int, 8>>& nodeChildren,
        const std::vector<std::array<bool, 8>>& nodeChildIsLeaf,
        const std::vector<std::array<physics::AABB, 8>>& nodeChildAABB)
{
    float dist = 0.0f;

    for (unsigned int i = 0; i < 8; ++i)
    {
        if (physics::rayAABBIntersection(ray.origin, ray.direction, nodeChildAABB[node][i].min, nodeChildAABB[node][i].max, dist))
        {
            if (nodeChildIsLeaf[node][i]) {
                buckets.push_back(nodeChildren.at(node).at(i));
            } else {
                getBuckets(nodeChildren[node][i], buckets, ray, nodeChildren, nodeChildIsLeaf, nodeChildAABB);
            }
        }
    }

}

template<unsigned int bucketSize>
bool physics::OctreeT<bucketSize>::trace(const Ray& ray, IntersectionDetails& result)
{
    float shortestDist = std::numeric_limits<float>::max();
    glm::vec3 closestResult;
    glm::vec3 v0, v1;
    bool intersection = false;

    bucketBuffer.clear();

    getBuckets(nodeRoot, bucketBuffer, ray, nodeChildren, nodeChildIsLeaf, nodeChildAABB);

    for (unsigned int i = 0; i < bucketBuffer.size(); ++i)
    {
        const auto& bucket = buckets[bucketBuffer[i]];
        const auto numTriangles = bucket.first;
        const auto& triangles = bucket.second;

        glm::vec3 intersectResult;
        for (unsigned int j = 0; j < numTriangles; ++j)
        {
            if (glm::intersectLineTriangle(
                    ray.origin,
                    ray.direction,
                    triangles[j].vertices[0], triangles[j].vertices[1], triangles[j].vertices[2],
                    intersectResult))
            {
                intersection = true;

                const float dist = intersectResult.x;
                if (dist < shortestDist)
                {
                    shortestDist = dist;
                    closestResult = intersectResult;

                    v0 = triangles[j].vertices[0];
                    v1 = triangles[j].vertices[1];
                }
            }
        }
    }

    if (intersection)
    {
        result.position = ray.origin + ray.direction * closestResult.x;
        result.normal = glm::cross(v0, v1);
        result.intersection = intersection;
    }

    return intersection;
}

template<unsigned int bucketSize>
void physics::OctreeT<bucketSize>::optimize()
{
    nodeChildren.clear();
    nodeChildIsLeaf.clear();
    nodeChildAABB.clear();
    buckets.clear();

    unsigned int internalNodes = 0, leaves = 0;
    countNodes(root, internalNodes, leaves);

    nodeChildren.resize(internalNodes);
    nodeChildIsLeaf.resize(internalNodes);
    nodeChildAABB.resize(internalNodes);
    buckets.reserve(leaves);

    nodeRoot = 0;

    insertDFS(root, objects, 0, nodeChildren, nodeChildIsLeaf, nodeChildAABB, buckets);
}

template class physics::OctreeT<32>;
