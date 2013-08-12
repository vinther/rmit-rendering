/*
 * BFSLayoutPolicy.cpp
 *
 *  Created on: 12/08/2013
 *      Author: svp
 */

#include "physics/detail/BFSLayoutPolicy.hpp"

#include <queue>

#include <SDL2/SDL_log.h>

#include "scene/SceneNode.hpp"

#include "physics/Octree.hpp"
#include "physics/detail/ConstructionTree.hpp"

void physics::detail::BFSLayoutPolicy::layout(
		const ConstructionTree& cTree,
		Octree::Data& data)
{
	unsigned int internalNodes = 0, leaves = 0;
	cTree.countNodes(internalNodes, leaves);

	data.aabbs.resize(internalNodes);
	data.children.resize(internalNodes);
	data.leaves.resize(internalNodes);

	data.descriptors.clear();
	data.objects.clear();

	std::queue<std::tuple<unsigned int, unsigned int, ConstructionTree::ConstructionNode const *>> q;

	unsigned int nodeIdx = 0;
	unsigned int parentIdx, childIdx;
	ConstructionTree::ConstructionNode const * nodePtr = nullptr;

	for (unsigned int i = 0; i < 8 ; ++i)
	{
		data.leaves[nodeIdx][i] = cTree.root->children[i]->isLeaf;
		data.aabbs[nodeIdx][i] = cTree.root->children[i]->aabb;

		q.push(std::make_tuple(nodeIdx, i, cTree.root->children[i].get()));
	}

	while (!q.empty())
	{
		++nodeIdx;

		std::tie(parentIdx, childIdx, nodePtr) = q.front();
		q.pop();

		data.children[parentIdx][childIdx] = nodeIdx;

		for (unsigned int i = 0; i < 8; ++i)
		{
			data.leaves[nodeIdx][i] = nodePtr->children[i]->isLeaf;
			data.aabbs[nodeIdx][i] = nodePtr->children[i]->aabb;

			if (data.leaves[nodeIdx][i])
			{
				if (0 == nodePtr->children[i]->bucket.size())
				{
					data.children[nodeIdx][i] = 0;
					continue;
				}

				BucketDescriptor descriptor;

				descriptor.size = nodePtr->children[i]->bucket.size();
				descriptor.offset = data.objects.size();

				data.children[nodeIdx][i] = data.descriptors.size();
				data.objects.resize(data.objects.size() + descriptor.size);

				for (unsigned int j = 0; j < descriptor.size; ++j)
					data.objects[descriptor.offset + j] = cTree.objects[nodePtr->children[i]->bucket[j]];

				data.descriptors.push_back(descriptor);
			} else
			{
				q.push(std::make_tuple(nodeIdx, i, nodePtr->children[i].get()));
			}
		}
	}
}


