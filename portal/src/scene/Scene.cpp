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
#include "assets/AssetManager.hpp"

#include "Utilities.hpp"

Scene::Scene(std::shared_ptr<Client> client)
    : camera(std::make_unique<Camera>())
    , assetManager(std::make_unique<AssetManager>())
    , client(client)
{
    // TODO Auto-generated constructor stub

    assetManager->initialize();
}

Scene::~Scene()
{
    // TODO Auto-generated destructor stub
}

