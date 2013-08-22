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

namespace threading
{
class ThreadPool;
}

typedef unsigned int GLuint;

namespace renderer
{

namespace resources
{
class Model;
class Shader;
class Texture;
class FrameBuffer;
class UniformBuffer;
}

class ResourceManager;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    struct Settings
    {
        enum Output
        {
            OUTPUT_FULL = 1,
            OUTPUT_DEPTH,
            OUTPUT_NORMALS,
            OUTPUT_ALBEDO,
            OUTPUT_POSITIONS,
            OUTPUT_AMBIENT_OCCLUSION,
        };

        Settings()
            : width(1280), height(720)
            , bumpMapping(false)
            , output(Output::OUTPUT_FULL)
        {}

        Uint16 width, height;

        bool bumpMapping;

        Output output;
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
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 normalMatrix;

        std::shared_ptr<const resources::Shader> activeShader;
        std::shared_ptr<const resources::UniformBuffer> activeUniformBuffer;
    };

    void initialize(SDL_Window* window, SDL_GLContext context, assets::AssetManager& assetManager);
    void prepareFrame(threading::ThreadPool& threadPool, SDL_Window* window, SDL_GLContext context);
    void render(const scene::Scene& scene, RenderResults& results);

    std::unique_ptr<ResourceManager> resourceManager;
private:
    void renderModel(const resources::Model& model, RenderState& state) const;
    void renderNode(const scene::SceneNode& node, RenderState& state) const;

    void fillGeometryBuffer(const scene::Scene& scene) const;
    void renderGeometryBuffer(const scene::Scene& scene) const;

    void renderFullscreenScreenQuad() const;

    std::shared_ptr<resources::Shader> deferredPassOneShader, deferredPassTwoShader;
    std::shared_ptr<resources::Shader> geometryBufferOutputShader;

    std::unique_ptr<resources::FrameBuffer> frameBuffer;
    std::shared_ptr<resources::UniformBuffer> materialBuffer;
};

}

#endif /* RENDERER_HPP_ */
