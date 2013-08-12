/*
 * Scene.hpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <memory>

#include "physics/Octree.hpp"

#include "scene/Camera.hpp"
#include "scene/SceneNode.hpp"

namespace scene
{

class Scene
{
public:
    Scene();
    virtual ~Scene();

    void initialize();

    std::unique_ptr<Camera> camera;
    std::unique_ptr<SceneNode> root;
    std::unique_ptr<physics::Octree> intersectionTree;
};

}

#endif /* SCENE_HPP_ */
