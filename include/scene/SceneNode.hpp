/*
 * SceneNode.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef SCENENODE_HPP_
#define SCENENODE_HPP_

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "physics/Octree.hpp"

namespace assets
{
class Model;
}

namespace scene
{

class SceneNode
{
public:
    SceneNode();
    virtual ~SceneNode();

    std::vector<std::shared_ptr<const assets::Model>> models;
    std::vector<std::unique_ptr<SceneNode>> children;

    glm::mat4 transformation;

    std::unique_ptr<physics::Octree> bvh;

    void initialize();
};

}

#endif /* SCENENODE_HPP_ */
