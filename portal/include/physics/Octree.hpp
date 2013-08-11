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

template <unsigned int bucketSize>
class OctreeT
{
public:
    OctreeT();
    ~OctreeT();

    struct Node
    {
        const static unsigned int nodeBucketSize = bucketSize;

        Node()
            : isLeaf(true)
        {}

        std::array<std::unique_ptr<Node>, 8> children;
        std::vector<unsigned int> bucket;

        AABB aabb;

        bool isLeaf;
    };

    std::unique_ptr<Node> root;
    std::vector<Triangle> objects;

    void createFromScene(const scene::SceneNode& sceneRoot);

    struct IntersectionDetails
    {
        glm::vec3 normal;
        glm::vec3 position;
        bool intersection;
    };

    bool trace(const Ray& ray, IntersectionDetails& result);

    unsigned int nodeRoot;

    std::vector<std::array<unsigned int, 8>> nodeChildren;
    std::vector<std::array<bool, 8>> nodeChildIsLeaf;
    std::vector<std::array<AABB, 8>> nodeChildAABB;
    std::vector<unsigned int> bucketSizes;
    std::vector<Triangle> buckets;


    glm::vec3 aabbMin;
    glm::vec3 aabbMax;

    void optimize();

private:
    std::vector<std::pair<unsigned int, unsigned int>> bucketBuffer;
};

typedef OctreeT<32> Octree;

}



#endif /* OCTREE_HPP_ */
