/*
 * Octree.cpp
 *
 *  Created on: 05/08/2013
 *      Author: svp
 */

#include "physics/Octree.hpp"

#include <assimp/scene.h>
#include <SDL2/SDL_log.h>

#include "physics/Intersections.hpp"
#include "assets/Model.hpp"
#include "scene/SceneNode.hpp"

#include "Utilities.hpp"

template <unsigned int bucketSize>
physics::Octree<bucketSize>::Octree()
{
}

template <unsigned int bucketSize>
physics::Octree<bucketSize>::~Octree()
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
    } else if (node.isLeaf && node.bucket.size() < 256)
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
        node.children[2]->aabb.min = min + glm::vec3(1.0f, 0.0f, 0.0f) * half;
        node.children[3]->aabb.min = min + glm::vec3(1.0f, 1.0f, 0.0f) * half;

        node.children[4]->aabb.min = min + glm::vec3(0.0f, 0.0f, 1.0f) * half;
        node.children[5]->aabb.min = min + glm::vec3(0.0f, 1.0f, 1.0f) * half;
        node.children[6]->aabb.min = min + glm::vec3(1.0f, 0.0f, 1.0f) * half;
        node.children[7]->aabb.min = min + glm::vec3(1.0f, 1.0f, 1.0f) * half;

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
void insertFaces(
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
        insertFaces<NodeType>(mesh.mFaces[i], mesh.mVertices, root, objects, startIdx + i);
    }
}

template <unsigned int bucketSize>
void physics::Octree<bucketSize>::createFromScene(const scene::SceneNode& sceneRoot)
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
}

template <typename NodeType, typename BucketType>
unsigned int insertBFS(
        const NodeType& ptrNode,
        const std::vector<physics::Triangle>& objects,

        unsigned int node,

        std::vector<std::array<unsigned int, 8>>& nodeChildren,
        std::vector<physics::AABB>& nodeAABB,
        std::vector<bool>& nodeIsLeaf,
        std::vector<BucketType>& bucket)
{
    nodeAABB[node] = ptrNode->aabb;
    nodeIsLeaf[node] = ptrNode->isLeaf;

    if (ptrNode->isLeaf)
    {
        unsigned int bucketIdx = bucket.size();

        bucket.emplace_back();
        bucket[bucketIdx].first = ptrNode->bucket.size();

        for (unsigned int i = 0; i < ptrNode->bucket.size(); ++i)
            bucket[node].second[i] = objects[ptrNode->bucket[i]];

        nodeChildren[node][0] = bucketIdx;

        return node + 1;
    }

    nodeChildren[node][0] = node + 1;
    nodeChildren[node][1] = node + 2;
    nodeChildren[node][2] = node + 3;
    nodeChildren[node][3] = node + 4;
    nodeChildren[node][4] = node + 5;
    nodeChildren[node][5] = node + 6;
    nodeChildren[node][6] = node + 7;
    nodeChildren[node][7] = node + 8;

    unsigned int nextIndex = 0;

    nextIndex = insertBFS(ptrNode->children[0], objects, nodeChildren[node][7], nodeChildren, nodeAABB, nodeIsLeaf, bucket);
    nextIndex = insertBFS(ptrNode->children[1], objects, nextIndex, nodeChildren, nodeAABB, nodeIsLeaf, bucket);
    nextIndex = insertBFS(ptrNode->children[2], objects, nextIndex, nodeChildren, nodeAABB, nodeIsLeaf, bucket);
    nextIndex = insertBFS(ptrNode->children[3], objects, nextIndex, nodeChildren, nodeAABB, nodeIsLeaf, bucket);
    nextIndex = insertBFS(ptrNode->children[4], objects, nextIndex, nodeChildren, nodeAABB, nodeIsLeaf, bucket);
    nextIndex = insertBFS(ptrNode->children[5], objects, nextIndex, nodeChildren, nodeAABB, nodeIsLeaf, bucket);
    nextIndex = insertBFS(ptrNode->children[6], objects, nextIndex, nodeChildren, nodeAABB, nodeIsLeaf, bucket);

    return insertBFS(ptrNode->children[7], objects, nextIndex, nodeChildren, nodeAABB, nodeIsLeaf, bucket);
}

template <typename NodeType, typename BucketType>
unsigned int insertDFS(
        const NodeType& ptrNode,
        const std::vector<physics::Triangle>& objects,

        unsigned int node,

        std::vector<std::array<unsigned int, 8>>& nodeChildren,
        std::vector<physics::AABB>& nodeAABB,
        std::vector<bool>& nodeIsLeaf,
        std::vector<BucketType>& bucket)
{
    nodeAABB[node] = ptrNode->aabb;
    nodeIsLeaf[node] = ptrNode->isLeaf;

    if (ptrNode->isLeaf)
    {
        unsigned int bucketIdx = bucket.size();

        bucket.emplace_back();
        bucket[bucketIdx].first = ptrNode->bucket.size();

        for (unsigned int i = 0; i < ptrNode->bucket.size(); ++i)
            bucket[node].second[i] = objects[ptrNode->bucket[i]];

        nodeChildren[node][0] = bucketIdx;

        return node + 1;
    }

    nodeChildren[node][0] = node + 1;

    for (unsigned int i = 1; i < 8; ++i)
        nodeChildren[node][i] = insertDFS(ptrNode->children[i - 1], objects, nodeChildren[node][i - 1], nodeChildren, nodeAABB, nodeIsLeaf, bucket);

    return insertDFS(ptrNode->children[7], objects, nodeChildren[node][7], nodeChildren, nodeAABB, nodeIsLeaf, bucket);
}

template <typename NodeType>
void countNodes(const NodeType& node, unsigned int& internalNodes, unsigned int& leaves)
{
    internalNodes += 1;

    if (node->isLeaf)
            leaves += 1;
    else
    {
        for (const auto& child: node->children)
        {
            countNodes(child, internalNodes, leaves);
        }
    }
}

template<unsigned int bucketSize>
void physics::Octree<bucketSize>::optimize()
{
    nodeChildren.clear();
    nodeAABB.clear();
    nodeIsLeaf.clear();
    buckets.clear();

    unsigned int internalNodes = 0, leaves = 0;
    countNodes(root, internalNodes, leaves);

    nodeChildren.resize(internalNodes);
    nodeAABB.resize(internalNodes);
    nodeIsLeaf.resize(internalNodes);
    buckets.reserve(leaves);

    nodeRoot = 0;

    insertDFS(root, objects, 0, nodeChildren, nodeAABB, nodeIsLeaf, buckets);
}

template class physics::Octree<256>;
