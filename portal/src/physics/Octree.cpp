/*
 * Octree.cpp
 *
 *  Created on: 05/08/2013
 *      Author: svp
 */

#include "physics/Octree.hpp"

#include <queue>
#include <stack>

#include <assimp/scene.h>
#include <SDL2/SDL_log.h>
#include <glm/gtx/intersect.hpp>

#include "physics/Intersections.hpp"
#include "physics/detail/ConstructionTree.hpp"

#include "assets/Model.hpp"
#include "scene/SceneNode.hpp"

#include "Utilities.hpp"

physics::Octree::Octree()
    : nodeRoot(0)
{
}

physics::Octree::~Octree()
{
}

template <class MemoryLayoutPolicy>
void physics::Octree::createFromScene(const scene::SceneNode& sceneRoot, unsigned int bucketSize)
{
	detail::ConstructionTree constructionTree(sceneRoot, bucketSize);

	MemoryLayoutPolicy mlp;
	mlp.layout(constructionTree, data);

	aabb = {constructionTree.root->aabb.min, constructionTree.root->aabb.max};
}

inline void getDescriptors(
        const physics::Ray& ray,
        const physics::Octree::Data& data,

        std::vector<physics::detail::BucketDescriptor>& descriptors)
{
    const glm::vec3 directionRec = 1.0f / ray.direction;
    std::vector<unsigned int> q;
    q.push_back(0);

    while (!q.empty())
    {
        unsigned int node = q[q.size() - 1]; q.pop_back();

        for (unsigned int i = 0; i < 8; ++i)
        {
            if (data.children[node][i] &&
                    physics::rayAABBIntersectionOpt(
                            ray.origin, directionRec,
                            data.aabbs[node][i].min,
                            data.aabbs[node][i].max))
            {
                if (data.leaves[node][i]) {
                    descriptors.push_back(data.descriptors[data.children[node][i]]);
                } else {
                    q.push_back(data.children[node][i]);
                }
            }
        }
    }
}

bool physics::Octree::trace(const Ray& ray, IntersectionPoint& result)
{
    float shortestDist = std::numeric_limits<float>::max();
    glm::vec3 closestResult;
    glm::vec3 v0, v1;
    bool intersection = false;

    bucketDescriptorBuffer.clear();

    getDescriptors(ray, data, bucketDescriptorBuffer);

//    unsigned int sumBufferSize = 0;
    for (unsigned int i = 0; i < bucketDescriptorBuffer.size(); ++i)
    {
//    	sumBufferSize += bucketDescriptorBuffer[i].size;
        for (unsigned int j = 0; j < bucketDescriptorBuffer[i].size; ++j)
        {
            const Triangle& tri = data.objects[bucketDescriptorBuffer[i].offset + j];

            glm::vec3 intersectResult;
            if (glm::intersectLineTriangle(
                    ray.origin,
                    ray.direction,
                    tri.vertices[0], tri.vertices[1], tri.vertices[2],
                    intersectResult))
            {
                intersection = true;

                const float dist = intersectResult.x;
                if (dist < shortestDist)
                {
                    shortestDist = dist;
                    closestResult = intersectResult;

                    v0 = tri.vertices[0];
                    v1 = tri.vertices[1];
                }
            }
        }
    }

	result.position = ray.origin + ray.direction * closestResult.x;
	result.normal = glm::cross(v0, v1);

//	SDL_Log("Avg.: %f", (float) sumBufferSize / bucketDescriptorBuffer.size());

    return intersection;
}

#include "physics/detail/BFSLayoutPolicy.hpp"
#include "physics/detail/DFSLayoutPolicy.hpp"

namespace physics
{
template void Octree::createFromScene<detail::BFSLayoutPolicy>(const scene::SceneNode&, unsigned int);
template void Octree::createFromScene<detail::DFSLayoutPolicy>(const scene::SceneNode&, unsigned int);
}
