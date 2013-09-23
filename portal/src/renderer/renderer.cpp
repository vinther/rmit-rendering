/*
 * Renderer.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "renderer/Renderer.hpp"

#include <stdexcept>
#include <chrono>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"
#include "scene/Camera.hpp"

#include "assets/DataStore.hpp"

#include "threading/ThreadPool.hpp"

#include "Utilities.hpp"

#define printOpenGLError() printOglError(__FILE__, __LINE__)

void printOglError(const char *file, int line)
{
    GLenum glErr;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
    }
}

renderer::Renderer::Renderer()
    : resourceManager(std::make_unique<storage_backend>())
    , geometryBuffer()
{

}

renderer::Renderer::~Renderer()
{
}

void renderer::Renderer::initialize(SDL_Window* window, SDL_GLContext context, assets::asset_store& assetManager)
{
    glewInit();

    SDL_GL_MakeCurrent(window, context);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);

    const auto geometryPassShaderAsset = assetManager.getOrCreate<assets::DiskFile>(
            "shaders/geometryPass", "shaders/geometryPass.vert", "shaders/geometryPass.frag");

    const auto ambientLightShaderAsset = assetManager.getOrCreate<assets::DiskFile>(
            "shaders/ambientLight", "shaders/fullscreenQuad.vert", "shaders/ambientLight.frag");

    const auto pointLightShaderAsset = assetManager.getOrCreate<assets::DiskFile>(
            "shaders/pointLight", "shaders/defaultLight.vert", "shaders/pointLight.frag");

    const auto geometryBufferOutputShaderAsset = assetManager.getOrCreate<assets::DiskFile>(
            "shaders/final", "shaders/fullscreenQuad.vert", "shaders/geometryBufferOutput.frag");


    geometryPassShader = std::make_shared<resources::ShaderProgram>();
    geometryPassShader->attachShader(resources::ShaderProgram::SHADER_TYPE_VERTEX, assetManager.getOrCreate<assets::DiskFile>("shaders/geometryPass.vert"));


//    geometryPassShader = resourceManager->getByAsset<resources::ShaderProgram>(geometryPassShaderAsset);
//    ambientLightShader = resourceManager->getByAsset<resources::ShaderProgram>(ambientLightShaderAsset);
//    pointLightShader = resourceManager->getByAsset<resources::ShaderProgram>(pointLightShaderAsset);
//    finalShader = resourceManager->getByAsset<resources::ShaderProgram>(geometryBufferOutputShaderAsset);

    geometryBuffer = std::make_unique<resources::frame_buffer>();

    auto DS = std::make_unique<resources::Texture>(nullptr);
    auto RT0 = std::make_unique<resources::Texture>(nullptr);
    auto RT1 = std::make_unique<resources::Texture>(nullptr);
    auto RT2 = std::make_unique<resources::Texture>(nullptr);
    auto RT3 = std::make_unique<resources::Texture>(nullptr);

    DS->createBlank(GL_DEPTH24_STENCIL8, settings.width, settings.height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, resources::Texture::MipMapMode::NO_MIPMAP);
    RT0->createBlank(GL_RGBA8, settings.width, settings.height, GL_RGBA, GL_UNSIGNED_BYTE, resources::Texture::MipMapMode::NO_MIPMAP);
    RT1->createBlank(GL_RG16F, settings.width, settings.height, GL_RG, GL_FLOAT, resources::Texture::MipMapMode::NO_MIPMAP);
    RT2->createBlank(GL_RGBA8, settings.width, settings.height, GL_RGBA, GL_UNSIGNED_BYTE, resources::Texture::MipMapMode::NO_MIPMAP);
    RT3->createBlank(GL_RGBA8, settings.width, settings.height, GL_RGBA, GL_UNSIGNED_BYTE, resources::Texture::MipMapMode::NO_MIPMAP);

    geometryBuffer->attach(std::move(DS), GL_DEPTH_STENCIL_ATTACHMENT);
    geometryBuffer->attach(std::move(RT0), GL_COLOR_ATTACHMENT0);
    geometryBuffer->attach(std::move(RT1), GL_COLOR_ATTACHMENT1);
    geometryBuffer->attach(std::move(RT2), GL_COLOR_ATTACHMENT2);
    geometryBuffer->attach(std::move(RT3), GL_COLOR_ATTACHMENT3);

    const auto pointLightAsset = assetManager.getOrCreate<assets::Mesh>("models/pointLight.obj");

    materialBuffer = std::make_shared<resources::UniformBuffer>();
    pointLights = std::make_unique<resources::PointLightGroup>();

    pointLights->buffer->bind(binding_points::POINT_LIGHT_BUFFER);

    std::vector<resources::PointLightGroup::LightData> pointLightData;

    pointLightData.resize(64);

    std::mt19937 gen;
    std::uniform_real_distribution<float> D;

    std::generate(std::begin(pointLightData), std::end(pointLightData), [&](){
        return resources::PointLightGroup::LightData{
                glm::vec4(D(gen) * 2000.0f - 1000.0f, D(gen) * 600.0f, D(gen) * 1000.0f - 500.0f, 0.0f),
                glm::vec4(D(gen), D(gen), D(gen), 1.0f),
                glm::vec4(D(gen) * 1200.0f, 0.0f, 0.0f, 0.0f)};
    });

    assert(sizeof(resources::PointLightGroup::LightData) == 3 * 4 * sizeof(float));

    pointLights->create(pointLightAsset, pointLightData);
    printOpenGLError();
}

float test = 0.0f;
unsigned int testInt = 0;


void renderer::Renderer::render(const scene_graph::scene_graph& scene, render_results& results)
{
    using namespace std::chrono;
    const auto timeBegin = high_resolution_clock::now();

    test += 0.01f;
    testInt += 1;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 239.0f / 255.0f, 0);

    doGeometryPass(scene);
    doLightPasses(scene);
    doPostProcessing(scene);

    finalizeOutput(scene);

    results.settings = settings;
    results.renderTime = duration_cast<microseconds>(high_resolution_clock::now() - timeBegin);
}

void renderer::Renderer::renderNode(const scene_graph::scene_node& node, render_state& state) const
{
    const glm::mat4 modelMatrix = state.modelMatrix;

    state.modelMatrix = state.modelMatrix * node.transformation;

    state.activeShader->setUniform("modelViewMatrix", state.viewMatrix * state.modelMatrix);
    geometryPassShader->setUniform("normalMatrix", glm::transpose(glm::inverse(state.viewMatrix * state.modelMatrix)));

    for (const auto& model: node.models)
    {
        renderModel(*(resourceManager->getByAsset<resources::Mesh>(model)), state);
    }

    for (auto& childNodePtr: node.children)
    {
        renderNode(*(childNodePtr), state);
    }

    state.modelMatrix = modelMatrix;
}

void renderer::Renderer::renderModel(const resources::Mesh& model, render_state& state) const
{
    if (!(model.state & resources::Mesh::State::READY))
        return;

    for (const auto& mesh: model.meshes)
    {
        glBindVertexArray(mesh.vao);

        auto& material = *(mesh.material);
        material.activate();

        state.activeUniformBuffer->bind(1);
        state.activeUniformBuffer->data(sizeof(glm::vec4) * 4 + sizeof(float), &material.materialInfo.emission);

        glDrawElements(GL_TRIANGLES, mesh.numFaces * 3, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}

void renderer::Renderer::doGeometryPass(const scene_graph::scene_graph& scene) const
{
    const auto& camera = *(scene.camera);

    geometryBuffer->enable({GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3});
    geometryPassShader->enable();
    materialBuffer->enable();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 239.0f / 255.0f, 0);
    glEnable(GL_DEPTH_TEST);

    {
        geometryPassShader->setUniform("emissiveTexSampler", resources::Material::MaterialInfo::TEXTURE_EMISSIVE);
        geometryPassShader->setUniform("ambientTexSampler", resources::Material::MaterialInfo::TEXTURE_AMBIENT);
        geometryPassShader->setUniform("diffuseTexSampler", resources::Material::MaterialInfo::TEXTURE_DIFFUSE);
        geometryPassShader->setUniform("specularTexSampler", resources::Material::MaterialInfo::TEXTURE_SPECULAR);
        geometryPassShader->setUniform("bumpTexSampler", resources::Material::MaterialInfo::TEXTURE_BUMP);
    }

    geometryPassShader->setUniform("projectionMatrix", camera.projection());
    geometryPassShader->setUniform("test", test);
    geometryPassShader->setUniform("enableBumpMapping", settings.bump_mapping);

    geometryPassShader->bindUniformBlock("MaterialParametersLoc", 1);

    render_state state{
        glm::mat4(1.0f), camera.view(), camera.projection(), glm::mat4(1.0f),
        geometryPassShader, materialBuffer
    };

    glEnableClientState(GL_VERTEX_ARRAY);

    renderNode(*(scene.root), state);

    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);

    geometryPassShader->disable();
    materialBuffer->disable();
    geometryBuffer->disable();
}

#include <GL/glut.h>

void renderer::Renderer::doLightPasses(const scene_graph::scene_graph& scene) const
{
    const auto& camera = *(scene.camera);

    ambientLightShader->enable();
    geometryBuffer->bindTextures({
        {GL_COLOR_ATTACHMENT1, GL_TEXTURE1},
        {GL_COLOR_ATTACHMENT2, GL_TEXTURE2},
        {GL_COLOR_ATTACHMENT3, GL_TEXTURE3},
        {GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE4}
    });

    for (const auto& t: {
            std::make_pair("RT1Sampler", 1),
            std::make_pair("RT2Sampler", 2),
            std::make_pair("RT3Sampler", 3),
            std::make_pair("DSSampler",  4)})
    {
        ambientLightShader->setUniform(t.first, t.second);
    }

    geometryBuffer->enable({GL_COLOR_ATTACHMENT0});

    ambientLightShader->enable();

    renderFullscreenScreenQuad();

    for(;false;)
    {
        // Set light parameters here
        //renderFullscreenScreenQuad();
    }

    pointLightShader->enable();

    for (const auto& t: {
            std::make_pair("RT1Sampler", 1),
            std::make_pair("RT2Sampler", 2),
            std::make_pair("RT3Sampler", 3),
            std::make_pair("DSSampler",  4)})
    {
        pointLightShader->setUniform(t.first, t.second);
    }

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    pointLightShader->setUniform("projectionMatrix", camera.projection());
    pointLightShader->setUniform("viewMatrix", camera.view());
    pointLightShader->setUniform("modelMatrix", glm::mat4(1.0f));
    pointLightShader->setUniform("viewProjectionInverse", glm::inverse(camera.viewProjection()));
    pointLightShader->bindUniformBlock("LightDataLoc", binding_points::POINT_LIGHT_BUFFER);

    pointLights->buffer->enable();
    glBindVertexArray(pointLights->meshData.vao);

    glDrawElementsInstanced(GL_TRIANGLES, pointLights->meshData.numFaces * 3, GL_UNSIGNED_INT, 0, pointLights->count);

    glBindVertexArray(0);
    pointLights->buffer->disable();

    assert(pointLights->meshData.numFaces && pointLights->count && pointLights->meshData.vao);

    printOpenGLError();

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//            activeShader->setUniform("enableAmbientOcclusion", settings.ambientOcclusion);
//            activeShader->setUniform("enableLighting", settings.lighting);
//            activeShader->setUniform("ambientOcclusionOnly", OutputMode::AMBIENT_OCCLUSION_ONLY == settings.output);

    geometryBuffer->disable();
}

void renderer::Renderer::doPostProcessing(const scene_graph::scene_graph& scene) const
{
    UNUSED(scene);
}

void renderer::Renderer::finalizeOutput(const scene_graph::scene_graph& scene) const
{
    const auto& camera = *(scene.camera);

    std::shared_ptr<const resources::ShaderProgram> activeShader = finalShader;
    activeShader->enable();

    geometryBuffer->disable();
    geometryBuffer->bindTextures({
        {GL_COLOR_ATTACHMENT0, GL_TEXTURE0},
        {GL_COLOR_ATTACHMENT1, GL_TEXTURE1},
        {GL_COLOR_ATTACHMENT2, GL_TEXTURE2},
        {GL_COLOR_ATTACHMENT3, GL_TEXTURE3},
        {GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE4}
    });

    for (const auto& t: {
            std::make_pair("RT0Sampler", 0),
            std::make_pair("RT1Sampler", 1),
            std::make_pair("RT2Sampler", 2),
            std::make_pair("RT3Sampler", 3),
            std::make_pair("DSSampler",  4)})
    {
        activeShader->setUniform(t.first, t.second);
    }

    activeShader->setUniform("viewProjectionInverse", glm::inverse(camera.viewProjection()));
    activeShader->setUniform("cameraPosition", camera.position);

    {
        switch (settings.output)
        {
        case settings_t::OutputMode::FULL:
            activeShader->setSubroutine("lightAccumulation", GL_FRAGMENT_SHADER); break;
        case settings_t::OutputMode::DEPTH_ONLY:
            activeShader->setSubroutine("depth", GL_FRAGMENT_SHADER); break;
        case settings_t::OutputMode::NORMALS_ONLY:
            activeShader->setSubroutine("normals", GL_FRAGMENT_SHADER); break;
        case settings_t::OutputMode::ALBEDO_ONLY:
            activeShader->setSubroutine("albedo", GL_FRAGMENT_SHADER); break;
        case settings_t::OutputMode::POSITIONS_ONLY:
            activeShader->setSubroutine("positions", GL_FRAGMENT_SHADER); break;
        default:
            break;
        }
    }

    renderFullscreenScreenQuad();

    activeShader->disable();
}

void renderer::Renderer::renderFullscreenScreenQuad() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(glm::ortho(0.0f, (float) settings.width, 0.0f, (float) settings.height)));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(glm::mat4(1.0f)));

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(settings.width, 0, 0);
        glTexCoord2f(1, 1); glVertex3f(settings.width, settings.height, 0);
        glTexCoord2f(0, 1); glVertex3f(0, settings.height, 0);
    glEnd();
}

void renderer::Renderer::prepareFrame(threading::thread_pool& threadPool, SDL_Window* window, SDL_GLContext context)
{
	UNUSED(threadPool);

	SDL_GL_MakeCurrent(window, context);
	resourceManager->updateResources();
}

void renderer::Renderer::settings_t::toggleBumpMapping()
{
    bump_mapping = !bump_mapping;
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_RENDERER, "Bump mapping %s", bump_mapping ? "ON" : "OFF");
}

void renderer::Renderer::settings_t::toggleAmbientOcclusion()
{
    ambient_occlusion = !ambient_occlusion;
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_RENDERER, "SSAO %s", ambient_occlusion ? "ON" : "OFF");
}

void renderer::Renderer::settings_t::toggleLighting()
{
    lighting = !lighting;
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_RENDERER, "Lighting %s", lighting ? "ON" : "OFF");
}

void renderer::Renderer::settings_t::setOutput(OutputMode output)
{
    this->output = output;

    auto getName = [](const OutputMode mode) -> std::string
    {
        switch (mode)
        {
        case FULL: return "Full shading"; break;
        case DEPTH_ONLY: return "Depth only"; break;
        case NORMALS_ONLY: return "Normals only"; break;
        case ALBEDO_ONLY: return "Albedo only"; break;
        case POSITIONS_ONLY: return "Positions only"; break;
        case AMBIENT_OCCLUSION_ONLY: return "Ambient occlusion only"; break;
        }

        return "UNKNOWN";
    };

    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_RENDERER, "Output mode: %s", getName(output).c_str());
}
