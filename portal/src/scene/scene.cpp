/*
 * Scene.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "scene/scene_graph.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "client/client.hpp"

#include "assets/scene.hpp"

#include "physics/Octree.hpp"

#include "shared/utilities.hpp"


scene::scene_graph::scene_graph()
    : main_camera(std::make_unique<camera>())
    , root(std::make_unique<scene_node>())
{
}

scene::scene_graph::~scene_graph()
{
    // TODO Auto-generated destructor stub
}

void scene::scene_graph::initialize()
{
    root->initialize();
}
