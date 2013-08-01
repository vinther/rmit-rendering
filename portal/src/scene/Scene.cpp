/*
 * Scene.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "scene/Scene.hpp"

#include <iostream>

#include "client/Client.hpp"
#include "scene/Camera.hpp"
#include "scene/SceneNode.hpp"

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Texture.hpp"

#include "Utilities.hpp"

#include <glm/gtc/matrix_transform.hpp>

Scene::Scene(std::shared_ptr<Client> client)
    : camera(std::make_unique<Camera>())
    , root(std::make_unique<SceneNode>())
    , assetManager(std::make_unique<AssetManager>())
    , client(client)
{
    // TODO Auto-generated constructor stub

    assetManager->initialize();

    root->model = assetManager->getOrCreate<Model>("models/sibenik/sibenik.obj");

    assetManager->getOrCreate<Texture>("dims", "models/capsule/capsule.png");

//    auto child = std::make_unique<SceneNode>();
//    child->model = assetManager->getOrCreate<Model>("models/shuttle.obj");
//    child->transformation = glm::translate(child->transformation, glm::vec3(0.0f, 0.0f, -50.0f));
//
//    root->children.push_back(std::move(child));
//
//    child = std::make_unique<SceneNode>();
//    child->model = assetManager->getOrCreate<Model>("models/shuttle.obj");
//    child->transformation = glm::translate(child->transformation, glm::vec3(0.0f, 0.0f, 50.0f));
//
//    root->children.push_back(std::move(child));
}

Scene::~Scene()
{
    // TODO Auto-generated destructor stub
}

