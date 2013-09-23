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

#include "scene/camera.hpp"

#include "renderer/detail/storage_backend.hpp"
//#include "renderer/resources/Texture.hpp"
//#include "renderer/resources/Material.hpp"
//#include "renderer/resources/ShaderProgram.hpp"
//#include "renderer/resources/FrameBuffer.hpp"
//#include "renderer/resources/UniformBuffer.hpp"
//#include "renderer/resources/PointLightGroup.hpp"

#include "scene/scene_graph.hpp"


namespace threading
{
class thread_pool;
}

typedef unsigned int GLuint;

namespace renderer
{

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    struct settings_t
    {
        enum output_mode
        {
            OUTPUT_MODE_FULL = 1,
            OUTPUT_MODE_DEPTH_ONLY,
            OUTPUT_MODE_NORMALS_ONLY,
            OUTPUT_MODE_ALBEDO_ONLY,
            OUTPUT_MODE_POSITIONS_ONLY,
            OUTPUT_MODE_AMBIENT_OCCLUSION_ONLY,
        };

        settings_t()
            : width(1280), height(720)
            , bump_mapping(true), ambient_occlusion(true), lighting(true)
            , output(OUTPUT_MODE_FULL)
        {}

        Uint16 width, height;

        bool bump_mapping;
        bool ambient_occlusion;
        bool lighting;

        void toggleBumpMapping();
        void toggleAmbientOcclusion();
        void toggleLighting();
        void setOutput(output_mode output);

        output_mode output;
    } settings;

    struct render_results
    {
		render_results()
			: settings()
			, renderTime(0)
		{}

        settings_t settings;
        std::chrono::microseconds renderTime;
    };

    struct render_state
    {
        glm::mat4 modelMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::mat4 normalMatrix;

        shader_program_handle active_shader;
        uniform_buffer_handle active_uniform_buffer;
    };

    void initialize(SDL_Window* window, SDL_GLContext context, assets::asset_store& assetManager);
    void prepareFrame(threading::thread_pool& threadPool, SDL_Window* window, SDL_GLContext context);
    void render(const scene::scene_graph& scene, render_results& results);

    std::unique_ptr<storage_backend> resourceManager;
private:
    void renderModel(const mesh_handle& model, render_state& state) const;
    void renderNode(const scene::scene_node& node, render_state& state) const;

    void doGeometryPass(const scene::scene_graph& scene) const;
    void doLightPasses(const scene::scene_graph& scene) const;
    void doPostProcessing(const scene::scene_graph& scene) const;
    void finalizeOutput(const scene::scene_graph& scene) const;

    void renderFullscreenScreenQuad() const;

    enum binding_points
    {
        CAMERA_POSITION = 1,
        VIEW_PROJECTION_MATRIX,
        INV_VIEW_PROJECTION_MATRIX,
        TIME,

        POINT_LIGHT_BUFFER,
    };

    shader_program_handle geometryPassShader;
    shader_program_handle ambientLightShader;
    shader_program_handle pointLightShader;
    shader_program_handle finalShader;

    frame_buffer_handle geometryBuffer;
    uniform_buffer_handle materialBuffer;
    uniform_buffer_handle geometryBufferSetup;
    std::unique_ptr<resources::PointLightGroup> pointLights;

    storage_backend backend;
};

}

#endif /* RENDERER_HPP_ */
