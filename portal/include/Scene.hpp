/*
 * Scene.hpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <memory>

#include "Camera.hpp"

class Scene
{
public:
    Scene();
    virtual ~Scene();

    const std::shared_ptr<Camera>& getCamera() const
    {
        return camera;
    }

    void setCamera(const std::shared_ptr<Camera>& camera)
    {
        this->camera = camera;
    }

    void loadObject(const std::string& path);

private:
    std::shared_ptr<Camera> camera;
};

#endif /* SCENE_HPP_ */
