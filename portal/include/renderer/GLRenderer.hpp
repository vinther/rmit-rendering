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

class Scene;
class SceneNode;
class Camera;

typedef unsigned int GLuint;

namespace assets
{
class AssetManager;
}

class GLResourceManager;
class GLBufferedModel;

class GLRenderer
{
public:
    GLRenderer();
    virtual ~GLRenderer();

    struct Settings
    {
        Settings()
            : width(640), height(480)
            , fov(65.0f), nearClip(1.0f), farClip(2000.0f)
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

    };

    void initialize();
    void render(const Scene& scene, RenderResults& results);

    std::unique_ptr<GLResourceManager> resourceManager;

    GLuint modelMatrixLoc, viewMatrixLoc, projectionMatrix;
    size_t shaderHash;
private:
    void renderModel(const GLBufferedModel& model);
    void renderNode(SceneNode& node, const Camera& camera, glm::mat4 modelMatrix);
};

#endif /* RENDERER_HPP_ */