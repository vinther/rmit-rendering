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

class Model;

class SceneNode
{
public:
    SceneNode();
    virtual ~SceneNode();

    std::weak_ptr<Model> model;
    glm::mat4 transformation;

    std::vector<std::unique_ptr<SceneNode>> children;
};

#endif /* SCENENODE_HPP_ */
