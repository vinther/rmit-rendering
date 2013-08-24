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

#include "assets/AssetManager.hpp"

#include "renderer/ResourceManager.hpp"
#include "renderer/resources/Model.hpp"
#include "renderer/resources/Texture.hpp"
#include "renderer/resources/Material.hpp"
#include "renderer/resources/Shader.hpp"
#include "renderer/resources/FrameBuffer.hpp"
#include "renderer/resources/UniformBuffer.hpp"

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
    : resourceManager(std::make_unique<ResourceManager>())
    , frameBuffer()
{
    // TODO Auto-generated constructor stub

}

renderer::Renderer::~Renderer()
{
    // TODO Auto-generated destructor stub
}

void renderer::Renderer::initialize(SDL_Window* window, SDL_GLContext context, assets::AssetManager& assetManager)
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

    const auto passOneShaderAsset = assetManager.getOrCreate<assets::Shader>(
            "shaders/passOne", "shaders/deferredPassOne.vert", "shaders/deferredPassOne.frag");

    const auto passTwoShaderAsset = assetManager.getOrCreate<assets::Shader>(
            "shaders/passTwo", "shaders/fullScreenQuad.vert", "shaders/deferredPassTwo.frag");

    deferredPassOneShader = resourceManager->getByAsset<resources::Shader>(passOneShaderAsset);
    deferredPassTwoShader = resourceManager->getByAsset<resources::Shader>(passTwoShaderAsset);

    const auto geometryBufferOutputShaderAsset = assetManager.getOrCreate<assets::Shader>(
            "shaders/geometryBufferOutput", "shaders/fullScreenQuad.vert", "shaders/geometryBufferOutput.frag");

    geometryBufferOutputShader = resourceManager->getByAsset<resources::Shader>(geometryBufferOutputShaderAsset);

    frameBuffer = std::make_unique<resources::FrameBuffer>();

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

    frameBuffer->attach(std::move(DS), GL_DEPTH_STENCIL_ATTACHMENT);
    frameBuffer->attach(std::move(RT0), GL_COLOR_ATTACHMENT0);
    frameBuffer->attach(std::move(RT1), GL_COLOR_ATTACHMENT1);
    frameBuffer->attach(std::move(RT2), GL_COLOR_ATTACHMENT2);
    frameBuffer->attach(std::move(RT3), GL_COLOR_ATTACHMENT3);

    materialBuffer = std::make_shared<resources::UniformBuffer>();
}

struct LightSourceParameters
{
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  glm::vec4 position;
} lights[8];

float test = 0.0f;
unsigned int testInt = 0;


void renderer::Renderer::render(const scene::Scene& scene, RenderResults& results)
{
    using namespace std::chrono;
    const auto timeBegin = high_resolution_clock::now();

    test += 0.01f;
    testInt += 1;

    materialBuffer->enable();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 239.0f / 255.0f, 0);

    fillGeometryBuffer(scene);
    renderGeometryBuffer(scene);

    materialBuffer->disable();

    results.settings = settings;
    results.renderTime = duration_cast<microseconds>(high_resolution_clock::now() - timeBegin);
}

void renderer::Renderer::renderNode(const scene::SceneNode& node, RenderState& state) const
{
    const glm::mat4 modelMatrix = state.modelMatrix;

    state.modelMatrix = state.modelMatrix * node.transformation;

    state.activeShader->setUniform("modelViewMatrix", state.viewMatrix * state.modelMatrix);
    state.activeShader->setUniform("normalMatrix", glm::transpose(glm::inverse(state.viewMatrix * state.modelMatrix)));

    for (const auto& model: node.models)
    {
        renderModel(*(resourceManager->getByAsset<resources::Model>(model)), state);
    }

    for (auto& childNodePtr: node.children)
    {
        renderNode(*(childNodePtr), state);
    }

    state.modelMatrix = modelMatrix;
}

void renderer::Renderer::renderModel(const resources::Model& model, RenderState& state) const
{
    if (!(model.state & resources::Model::State::READY))
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

void renderer::Renderer::fillGeometryBuffer(const scene::Scene& scene) const
{
    const auto& camera = *(scene.camera);

    deferredPassOneShader->enable();
    frameBuffer->enable();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 239.0f / 255.0f, 0);
    glClearStencil(0);
    glEnable(GL_STENCIL_TEST);

    {
        using TextureTypes = resources::Material::MaterialInfo::TextureBuffers;

        deferredPassOneShader->setUniform("emissiveTexSampler", TextureTypes::TEXTURE_EMISSIVE);
        deferredPassOneShader->setUniform("ambientTexSampler", TextureTypes::TEXTURE_AMBIENT);
        deferredPassOneShader->setUniform("diffuseTexSampler", TextureTypes::TEXTURE_DIFFUSE);
        deferredPassOneShader->setUniform("specularTexSampler", TextureTypes::TEXTURE_SPECULAR);
        deferredPassOneShader->setUniform("bumpTexSampler", TextureTypes::TEXTURE_BUMP);
    }

    deferredPassOneShader->setUniform("projectionMatrix", camera.projection());
    deferredPassOneShader->setUniform("test", test);
    deferredPassOneShader->setUniform("enableBumpMapping", settings.bumpMapping);

    deferredPassOneShader->bindUniformBlock("MaterialParametersLoc", 1);

    RenderState state{
        glm::mat4(1.0f), camera.view(), camera.projection(), glm::mat4(1.0f),
        deferredPassOneShader, materialBuffer
    };

    glEnableClientState(GL_VERTEX_ARRAY);

    renderNode(*(scene.root), state);

    glDisableClientState(GL_VERTEX_ARRAY);

    glDisable(GL_STENCIL_TEST);

    frameBuffer->disable();
    deferredPassOneShader->disable();
}

void renderer::Renderer::renderGeometryBuffer(const scene::Scene& scene) const
{
    const auto& camera = *(scene.camera);

    glDisable(GL_DEPTH_TEST);

    std::shared_ptr<resources::Shader> activeShader;

    {
        using OutputMode = Settings::OutputMode;

        if (OutputMode::FULL == settings.output || OutputMode::AMBIENT_OCCLUSION_ONLY == settings.output)
        {
            activeShader = deferredPassTwoShader;
            activeShader->enable();

            activeShader->setUniform("enableAmbientOcclusion", settings.ambientOcclusion);
            activeShader->setUniform("enableLighting", settings.lighting);
            activeShader->setUniform("ambientOcclusionOnly", OutputMode::AMBIENT_OCCLUSION_ONLY == settings.output);
        } else
        {
            activeShader = geometryBufferOutputShader;
            activeShader->enable();

            switch (settings.output)
            {
            case OutputMode::DEPTH_ONLY:
                activeShader->setSubroutine("depth", GL_FRAGMENT_SHADER); break;
            case OutputMode::NORMALS_ONLY:
                activeShader->setSubroutine("normals", GL_FRAGMENT_SHADER); break;
            case OutputMode::ALBEDO_ONLY:
                activeShader->setSubroutine("albedo", GL_FRAGMENT_SHADER); break;
            case OutputMode::POSITIONS_ONLY:
                activeShader->setSubroutine("positions", GL_FRAGMENT_SHADER); break;
            default:
                break;
            }
        }
    }

    activeShader->setUniform("RT0Sampler", 0);
    activeShader->setUniform("RT1Sampler", 1);
    activeShader->setUniform("RT2Sampler", 2);
    activeShader->setUniform("RT3Sampler", 3);
    activeShader->setUniform("DSSampler", 4);

    activeShader->setUniform("viewProjectionInverse", glm::inverse(camera.viewProjection()));
    activeShader->setUniform("cameraPosition", camera.position);

    frameBuffer->bindTextures({
            {GL_COLOR_ATTACHMENT0, GL_TEXTURE0},
            {GL_COLOR_ATTACHMENT1, GL_TEXTURE1},
            {GL_COLOR_ATTACHMENT2, GL_TEXTURE2},
            {GL_COLOR_ATTACHMENT3, GL_TEXTURE3},
            {GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE4}
    });

    activeShader->setUniform("test", test);

    if (testInt >= 200) {
        activeShader->setUniform("pingStart", camera.position);
        testInt = 0;
    }

    renderFullscreenScreenQuad();

    activeShader->disable();

    glEnable(GL_DEPTH_TEST);
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

void renderer::Renderer::prepareFrame(threading::ThreadPool& threadPool, SDL_Window* window, SDL_GLContext context)
{
	UNUSED(threadPool);

	SDL_GL_MakeCurrent(window, context);
	resourceManager->updateResources();
}

void renderer::Renderer::Settings::toggleBumpMapping()
{
    bumpMapping = !bumpMapping;
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_RENDERER, "Bump mapping %s", bumpMapping ? "ON" : "OFF");
}

void renderer::Renderer::Settings::toggleAmbientOcclusion()
{
    ambientOcclusion = !ambientOcclusion;
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_RENDERER, "SSAO %s", ambientOcclusion ? "ON" : "OFF");
}

void renderer::Renderer::Settings::toggleLighting()
{
    lighting = !lighting;
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_RENDERER, "Lighting %s", lighting ? "ON" : "OFF");
}

void renderer::Renderer::Settings::setOutput(OutputMode output)
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
