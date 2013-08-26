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

#include "renderer/ResourceManager.hpp"
#include "renderer/resources/Model.hpp"
#include "renderer/resources/Texture.hpp"
#include "renderer/resources/Material.hpp"
#include "renderer/resources/Shader.hpp"
#include "renderer/resources/FrameBuffer.hpp"
#include "renderer/resources/UniformBuffer.hpp"
#include "renderer/resources/PointLightGroup.hpp"

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

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    struct Settings
    {
        enum OutputMode
        {
            FULL = 1,
            DEPTH_ONLY,
            NORMALS_ONLY,
            ALBEDO_ONLY,
            POSITIONS_ONLY,
            AMBIENT_OCCLUSION_ONLY,
        };

        Settings()
            : width(1280), height(720)
            , bumpMapping(true), ambientOcclusion(true), lighting(true)
            , output(OutputMode::FULL)
        {}

        Uint16 width, height;

        bool bumpMapping;
        bool ambientOcclusion;
        bool lighting;

        void toggleBumpMapping();
        void toggleAmbientOcclusion();
        void toggleLighting();
        void setOutput(OutputMode output);

        OutputMode output;
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

        std::shared_ptr<const resources::ShaderProgram> activeShader;
        std::shared_ptr<const resources::UniformBuffer> activeUniformBuffer;
    };

    void initialize(SDL_Window* window, SDL_GLContext context, assets::AssetManager& assetManager);
    void prepareFrame(threading::ThreadPool& threadPool, SDL_Window* window, SDL_GLContext context);
    void render(const scene::Scene& scene, RenderResults& results);

    std::unique_ptr<ResourceManager> resourceManager;
private:
    void renderModel(const resources::Model& model, RenderState& state) const;
    void renderNode(const scene::SceneNode& node, RenderState& state) const;

    void doGeometryPass(const scene::Scene& scene) const;
    void doLightPasses(const scene::Scene& scene) const;
    void doPostProcessing(const scene::Scene& scene) const;
    void finalizeOutput(const scene::Scene& scene) const;

    void renderFullscreenScreenQuad() const;

    enum BindingPoints
    {
        CAMERA_POSITION = 1,
        VIEW_PROJECTION_MATRIX,
        INV_VIEW_PROJECTION_MATRIX,
        TIME,

        POINT_LIGHT_BUFFER,
    };



    std::shared_ptr<resources::ShaderProgram> geometryPassShader;
    std::shared_ptr<resources::ShaderProgram> ambientLightShader;
    std::shared_ptr<resources::ShaderProgram> pointLightShader;
    std::shared_ptr<resources::ShaderProgram> finalShader;

    std::unique_ptr<resources::FrameBuffer> geometryBuffer;
    std::shared_ptr<resources::UniformBuffer> materialBuffer;
    std::shared_ptr<resources::UniformBuffer> geometryBufferSetup;
    std::unique_ptr<resources::PointLightGroup> pointLights;
};

}

#endif /* RENDERER_HPP_ */
