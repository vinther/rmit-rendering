/*
 * Scene.hpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <memory>

#include "scene/Camera.hpp"
#include "assets/AssetManager.hpp"

class Scene
{
public:
    Scene();
    virtual ~Scene();

public:
    std::shared_ptr<Camera> camera;
    std::shared_ptr<AssetManager> assetManager;
};

#endif /* SCENE_HPP_ */
