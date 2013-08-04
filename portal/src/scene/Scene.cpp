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
    , assetManager(std::make_unique<assets::AssetManager>())
    , client(client)
{
    assetManager->initialize();

    root->model = assetManager->getOrCreate<assets::Model>("models/sibenik/sibenik.obj", std::ref(*assetManager));

    //root->model = assetManager->getOrCreate<assets::Model>("models/capsule/capsule.obj", std::ref(*assetManager));
    //root->model = assetManager->getOrCreate<Model>("models/dabrovic-sponza/sponza.obj");
    //root->model = assetManager->getOrCreate<assets::Model>("models/portal/Portal Gun.obj", std::ref(*assetManager));
    //assetManager->getOrCreate<Texture>("dims", "models/capsule/capsule.png");
    //assetManager->getOrCreate<Texture>("dims", "models/portal/textures/portalgun_col.jpg");

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

