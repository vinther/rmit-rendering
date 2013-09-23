/*
 * ConstructionTree.cpp
 *
 *  Created on: 12/08/2013
 *      Author: svp
 */

#include "physics/detail/ConstructionTree.hpp"

#include "physics/Intersections.hpp"

#include "scene/SceneNode.hpp"

#include "assets/Model.hpp"

#include "Utilities.hpp"

physics::detail::ConstructionTree::ConstructionTree(const scene::scene_node& node, unsigned int bucketSize)
	: bucketSize(bucketSize)
{
    const float limitMin = std::numeric_limits<float>::min();
    const float limitMax = std::numeric_limits<float>::max();

    glm::vec3 min(limitMax, limitMax, limitMax);
    glm::vec3 max(limitMin, limitMin, limitMin);

    unsigned int totalNumFaces = 0;
    for (const auto& model: node.models)
    {
        for (unsigned int i = 0; i < model->scene->mNumMeshes; ++i)
        {
            const auto& mesh = *(model->scene->mMeshes[i]);

            for (unsigned int j = 0; j < mesh.mNumVertices; ++j)
            {
                const auto& v = mesh.mVertices[j];

                min = glm::vec3(glm::min(min.x, v.x), glm::min(min.y, v.y), glm::min(min.z, v.z));
                max = glm::vec3(glm::max(max.x, v.x), glm::max(max.y, v.y), glm::max(max.z, v.z));
            }

            totalNumFaces += mesh.mNumFaces;
        }
    }

    min = min - glm::vec3(1.0f, 1.0f, 1.0f);
    max = max + glm::vec3(1.0f, 1.0f, 1.0f);

    objects.reserve(totalNumFaces);

    root = std::make_unique<ConstructionNode>();
    root->aabb.min = min;
    root->aabb.max = max;

    for (const auto& model: node.models)
    {
        for (unsigned int i = 0; i < model->scene->mNumMeshes; ++i)
        {
            const auto& mesh = *(model->scene->mMeshes[i]);

            insertMesh(mesh);
        }
    }
}

void physics::detail::ConstructionTree::insertTriangleIndex(
		unsigned int idx,
		const Triangle& triangle,
		ConstructionNode& node)
{
	if (!physics::triangleAABBIntersection(triangle, node.aabb))
		return;

	if (!node.isLeaf)
	{
		for (auto& child: node.children)
		{
			insertTriangleIndex(idx, triangle, *child);
		}
	} else if (node.isLeaf && node.bucket.size() < bucketSize)
	{
		node.bucket.push_back(idx);
	} else
	{
		for (auto& child: node.children)
		{
			child = std::make_unique<ConstructionNode>();
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
				insertTriangleIndex(triangleIdx, objects[triangleIdx], *child);

			insertTriangleIndex(idx, triangle, *child);
		}

		node.bucket.empty();
		node.isLeaf = false;
	}
}

void physics::detail::ConstructionTree::insertFace(const aiFace& face, const aiVector3D* vertices)
{
    physics::Triangle triangle;

    const aiVector3D& v0 = vertices[face.mIndices[0]];
    const aiVector3D& v1 = vertices[face.mIndices[1]];
    const aiVector3D& v2 = vertices[face.mIndices[2]];

    triangle.vertices[0] = glm::vec3(v0.x, v0.y, v0.z);
    triangle.vertices[1] = glm::vec3(v1.x, v1.y, v1.z);
    triangle.vertices[2] = glm::vec3(v2.x, v2.y, v2.z);

    const unsigned int idx = objects.size();
    objects.push_back(triangle);

    insertTriangleIndex(idx, triangle, *(root));
}

void physics::detail::ConstructionTree::insertMesh(const aiMesh& mesh)
{
    for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
    {
        insertFace(mesh.mFaces[i], mesh.mVertices);
    }
}



void countNodesRecursive(
		const std::unique_ptr<physics::detail::ConstructionTree::ConstructionNode>& node,
		unsigned int& internalNodes,
		unsigned int& leaves)
{
	if (node->isLeaf)
	{
		leaves += 1;
	} else
	{
		internalNodes += 1;

		for (const auto& child: node->children)
		{
			countNodesRecursive(child, internalNodes, leaves);
		}
	}
}

void physics::detail::ConstructionTree::countNodes(unsigned int& internalNodes, unsigned int& leaves) const
{
	countNodesRecursive(root, internalNodes, leaves);
}
