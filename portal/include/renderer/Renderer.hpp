/*
 * Renderer.hpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <memory>
#include <vector>
#include <chrono>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "scene/Camera.hpp"

namespace scene
{
class Scene;
class SceneNode;
}

namespace assets
{
class AssetManager;
}

typedef unsigned int GLuint;

namespace renderer
{

class ResourceManager;
class BufferedModel;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    struct Settings
    {
        Settings()
            : width(640), height(480)
            , fov(65.0f), nearClip(1.0f), farClip(10000.0f)
        {}

        Uint16 width, height;
        float fov, nearClip, farClip;
    } settings;

    struct RenderResults
    {
		RenderResults()
			: settings()
			, renderTime(0)
		{}

        Settings settings;
        std::chrono::microseconds renderTime;
    };

    struct RenderState
    {
        scene::Camera::State cameraState;

        glm::mat4 modelMatrix;

        struct
        {
            int modelMatrix;
        } locations;
    };

    void initialize();
    void render(const scene::Scene& scene, RenderResults& results);

    std::unique_ptr<ResourceManager> resourceManager;

    size_t shaderHash;
private:
    void renderModel(const BufferedModel& model);
    void renderNode(scene::SceneNode& node, RenderState state);
};

}

#endif /* RENDERER_HPP_ */
