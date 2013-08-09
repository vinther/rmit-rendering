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

#include "physics/Octree.hpp"

namespace scene
{

class Camera;
class SceneNode;

class Scene
{
public:
    Scene(std::shared_ptr<Client> client);
    virtual ~Scene();

    void initialize();

    std::unique_ptr<Camera> camera;
    std::unique_ptr<SceneNode> root;
    std::unique_ptr<physics::Octree > intersectionTree;
private:
    std::shared_ptr<Client> client;
};

}

#endif /* SCENE_HPP_ */
