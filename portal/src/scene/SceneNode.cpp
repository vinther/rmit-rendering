/*
 * SceneNode.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "scene/SceneNode.hpp"

#include "assets/Model.hpp"

#include "Utilities.hpp"

scene::SceneNode::SceneNode()
    : models()
    , children()
    , transformation(glm::mat4(1.0f))
{
    // TODO Auto-generated constructor stub

}

scene::SceneNode::~SceneNode()
{
    // TODO Auto-generated destructor stub
}

void scene::SceneNode::initialize()
{
    bvh = std::make_unique<physics::Octree>();
    bvh->createFromNode(*this, 32);
}
