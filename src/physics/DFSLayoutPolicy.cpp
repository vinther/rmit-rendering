/*
 * DFSLayoutPolicy.cpp
 *
 *  Created on: 13/08/2013
 *      Author: svp
 */

#include "physics/detail/DFSLayoutPolicy.hpp"

#include <queue>

#include <SDL2/SDL_log.h>

#include "scene/SceneNode.hpp"

#include "physics/Octree.hpp"
#include "physics/detail/ConstructionTree.hpp"

//unsigned int layoutRecursively(
//		const physics::detail::ConstructionTree& cTree,
//		const std::unique_ptr<physics::detail::ConstructionTree::ConstructionNode>& ptrNode,
//		unsigned int node,
//
//		physics::Octree::Data& data)
//{
//    for (unsigned int i = 0; i < 8; ++i)
//    {
//        data.leaves[node][i] = ptrNode->children[i]->isLeaf;
//        data.aabbs[node][i] = ptrNode->children[i]->aabb;
//
//        if (!data.leaves[node][i])
//            continue;
//
//        physics::detail::BucketDescriptor descriptor;
//
//        descriptor.size = ptrNode->children[i]->bucket.size();
//        descriptor.offset = data.objects.size();
//
//        data.children[node][i] = data.descriptors.size();
//
//        data.descriptors.push_back(descriptor);
//
//        data.objects.reserve(descriptor.offset + descriptor.size);
//
//        for (unsigned int j = 0; j < descriptor.size; ++j)
//        	data.objects[descriptor.offset + j] = cTree.objects[ptrNode->children[i]->bucket[j]];
//    }
//
//    unsigned int nextIdx = node + 1;
//    for (unsigned int i = 0; i < 8; ++i)
//    {
//        if (!data.leaves[node][i])
//        {
//            data.children[node][i] = nextIdx;
//
//            nextIdx = layoutRecursively(cTree, ptrNode->children[i], nextIdx, data);
//        }
//    }
//
//    return nextIdx;
//}
//
//void physics::detail::DFSLayoutPolicy::layout(
//		const ConstructionTree& cTree,
//		Octree::Data& data)
//{
//	layoutRecursively(cTree, cTree.root, 0, data);
//}
