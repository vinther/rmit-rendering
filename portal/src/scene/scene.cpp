/*
 * Scene.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "scene/scene.hpp"

#include <iostream>

#include "client/client.hpp"

#include "assets/scene.hpp"

#include "physics/Octree.hpp"

#include "shared/utilities.hpp"

#include <glm/gtc/matrix_transform.hpp>

scene_graph::scene_graph::scene_graph()
    : camera(std::make_unique<camera>())
    , root(std::make_unique<scene_node>())
{
}

scene_graph::scene_graph::~scene_graph()
{
    // TODO Auto-generated destructor stub
}

void scene_graph::scene_graph::initialize()
{
    root->initialize();
}
