/*
 * Scene.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "scene/Scene.hpp"

#include <iostream>

#include "client/Client.hpp"

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Texture.hpp"

#include "physics/Octree.hpp"

#include "Utilities.hpp"

#include <glm/gtc/matrix_transform.hpp>

scene::Scene::Scene()
    : camera(std::make_unique<Camera>())
    , root(std::make_unique<SceneNode>())
    , intersectionTree(std::make_unique<physics::Octree >())
{
}

scene::Scene::~Scene()
{
    // TODO Auto-generated destructor stub
}

void scene::Scene::initialize()
{

}
