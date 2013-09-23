#ifndef SCENE_SCENE_NODE_HPP_
#define SCENE_SCENE_NODE_HPP_

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "physics/Octree.hpp"

#include "assets/scene.hpp"

namespace scene
{

class scene_node
{
public:
    scene_node();
    virtual ~scene_node();

    std::vector<std::shared_ptr<const assets::scene>> models;
    std::vector<std::unique_ptr<scene_node>> children;

    glm::mat4 transformation;

    std::unique_ptr<physics::Octree> bvh;

    void initialize();
};

}

#endif /* SCENE_SCENE_NODE_HPP_ */
