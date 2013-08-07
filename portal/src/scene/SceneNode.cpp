/*
 * SceneNode.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "scene/SceneNode.hpp"

#include "assets/Model.hpp"

scene::SceneNode::SceneNode()
    : model()
    , transformation(glm::mat4(1.0f))
    , children()
{
    // TODO Auto-generated constructor stub

}

scene::SceneNode::~SceneNode()
{
    // TODO Auto-generated destructor stub
}

