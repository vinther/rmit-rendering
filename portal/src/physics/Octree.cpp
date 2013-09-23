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
void physics::Octree::createFromNode(const scene::scene_node& node, unsigned int bucketSize)
{
	detail::ConstructionTree constructionTree(node, bucketSize);

	MemoryLayoutPolicy mlp;
	mlp.layout(constructionTree, data);

	aabb = {constructionTree.root->aabb.min, constructionTree.root->aabb.max};
}

inline void getDescriptors(
        const physics::SIMDRay ray,
        const physics::Octree::Data& data,

        std::vector<physics::detail::BucketDescriptor>& descriptors)
{
    const glm::simdVec4 directionRec = 1.0f / ray.direction;

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

    bool intersection = false;

    glm::simdVec4 v0, v1;
    const glm::simdVec4 o = glm::simdVec4(ray.origin, 0.0f);
    const glm::simdVec4 d = glm::simdVec4(ray.origin, 1.0f);

    bucketDescriptorBuffer.clear();

    getDescriptors({o, d}, data, bucketDescriptorBuffer);

    SDL_Log("%d", bucketDescriptorBuffer.size());

    return false;
//    unsigned int sumBufferSize = 0;
    for (unsigned int i = 0; i < bucketDescriptorBuffer.size(); ++i)
    {
//    	sumBufferSize += bucketDescriptorBuffer[i].size;
        for (unsigned int j = 0; j < bucketDescriptorBuffer[i].size; ++j)
        {
            const SIMDTriangle& tri = data.objects[bucketDescriptorBuffer[i].offset + j];

            float t;
            if (lineTriangleIntersection(
                    o, d,
                    tri[0], tri[1], tri[2],
                    t))
            {
                intersection = true;

                float dist = 0.0f;
                if (t < shortestDist)
                {
                    shortestDist = dist;

                    v0 = tri.vertices[0];
                    v1 = tri.vertices[1];
                }
            }
        }
    }

	result.position = ray.origin + ray.direction * shortestDist;
	result.normal = glm::vec3(glm::vec4_cast(glm::cross(v0, v1)));

	SDL_Log("Avg.: %f %f %f", result.position.x, result.position.y, result.position.z);

    return intersection;
}

bool physics::Octree::trace(const Ray& ray, const glm::mat4 transformation, IntersectionPoint& result)
{
    const glm::mat4 invTransformation = glm::inverse(transformation);

    Ray transformedRay;

    transformedRay.origin = glm::vec3(invTransformation * glm::vec4(ray.origin, 0.0f));
    transformedRay.direction = glm::vec3(invTransformation * glm::vec4(ray.direction, 0.0f));

    IntersectionPoint objectResult;

    bool hit = trace(ray, objectResult);

    result.position = glm::vec3(transformation * glm::vec4(objectResult.position, 0.0f));
    result.normal = glm::vec3(transformation * glm::vec4(objectResult.normal, 0.0f));

    return hit;
}

#include "physics/detail/BFSLayoutPolicy.hpp"
#include "physics/detail/DFSLayoutPolicy.hpp"

namespace physics
{
template void Octree::createFromNode<detail::BFSLayoutPolicy>(const scene::scene_node&, unsigned int);
//template void Octree::createFromNode<detail::DFSLayoutPolicy>(const scene::SceneNode&, unsigned int);
}


