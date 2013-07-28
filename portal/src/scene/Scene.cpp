/*
 * Scene.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "scene/Scene.hpp"

#include <iostream>

Scene::Scene()
    : camera(std::make_shared<Camera>())
    , assetManager(std::make_shared<AssetManager>())
{
    // TODO Auto-generated constructor stub

}

Scene::~Scene()
{
    // TODO Auto-generated destructor stub
}

