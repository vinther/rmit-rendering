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

template <unsigned int bucketSize = 256>
class Octree
{
public:
    Octree();
    ~Octree();

    struct Node
    {
        std::array<std::unique_ptr<Node>, 8> children;
        std::vector<unsigned int> bucket;

        AABB aabb;

        bool isLeaf = true;
    };

    std::unique_ptr<Node> root;

    std::vector<Triangle> objects;

    void createFromScene(const scene::SceneNode& sceneRoot);

    unsigned int nodeRoot;

    std::vector<std::array<unsigned int, 8>> nodeChildren;
    std::vector<AABB> nodeAABB;
    std::vector<bool> nodeIsLeaf;
    std::vector<std::pair<unsigned int, std::array<Triangle, bucketSize>>> buckets;

    void optimize();
};

}



#endif /* OCTREE_HPP_ */
