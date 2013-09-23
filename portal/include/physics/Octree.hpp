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
class scene_node;
}

namespace physics
{

namespace detail
{
struct bucket_descriptor
{
	unsigned int offset;
	unsigned int size;
};

struct BFSLayoutPolicy;
struct DFSLayoutPolicy;
struct VEBLayoutPolicy;

typedef BFSLayoutPolicy DefaultLayoutPolicy;

}

class octree
{
public:
    octree();
    ~octree();

    template <class MemoryLayoutPolicy = detail::DefaultLayoutPolicy>
    void from_node(const scene::scene_node& node, unsigned int bucketSize = 16);

    bool trace(const Ray& ray, IntersectionPoint& result);
    bool trace(const Ray& ray, const glm::mat4 transformation, IntersectionPoint& result);

    AABB aabb;

    unsigned int root;

    struct Data
    {
        std::vector<std::array<unsigned int, 8>> children;
        std::vector<std::array<bool, 8>> leaves;
        std::vector<std::array<SIMDAABB, 8>> aabbs;

        std::vector<detail::bucket_descriptor> descriptors;
        std::vector<SIMD_triangle> objects;
    } data;
private:
    std::vector<detail::bucket_descriptor> bucketDescriptorBuffer;
};

}

#endif /* OCTREE_HPP_ */
