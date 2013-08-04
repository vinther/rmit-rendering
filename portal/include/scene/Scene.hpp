/*
 * Scene.hpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <memory>

class Client;
class Camera;

class SceneNode;

namespace assets
{
class AssetManager;
}

class Scene
{
public:
    Scene(std::shared_ptr<Client> client);
    virtual ~Scene();

    std::unique_ptr<Camera> camera;
    std::unique_ptr<SceneNode> root;
    std::unique_ptr<assets::AssetManager> assetManager;
private:
    std::shared_ptr<Client> client;
};

#endif /* SCENE_HPP_ */
