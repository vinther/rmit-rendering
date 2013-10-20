/*
 * Octree.hpp
 *
 *  Created on: 05/08/2013
 *      Author: svp
 */

#ifndef OCTREE_HPP_
#define OCTREE_HPP_

#include <array>
#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include "physics/Types.hpp"

namespace scene
{
class SceneNode;
}

namespace physics
{

namespace detail
{
struct BucketDescriptor
{
	unsigned int offset;
	unsigned int size;
};

struct BFSLayoutPolicy;
struct DFSLayoutPolicy;
struct VEBLayoutPolicy;

typedef BFSLayoutPolicy DefaultLayoutPolicy;

}

class Octree
{
public:
    Octree();
    ~Octree();

    template <class MemoryLayoutPolicy = detail::DefaultLayoutPolicy>
    void createFromNode(const scene::SceneNode& node, unsigned int bucketSize = 256);

    bool trace(const Ray& ray, IntersectionPoint& result);
    bool trace(const Ray& ray, const glm::mat4 transformation, IntersectionPoint& result);

    AABB aabb;

    unsigned int nodeRoot;

    struct Data
    {
        std::vector<std::array<unsigned int, 8>> children;
        std::vector<std::array<bool, 8>> leaves;
        std::vector<std::array<AABB, 8>> aabbs;

        std::vector<detail::BucketDescriptor> descriptors;
        std::vector<Triangle> objects;
    } data;
private:
    std::vector<detail::BucketDescriptor> bucketDescriptorBuffer;
};

}

#endif /* OCTREE_HPP_ */
