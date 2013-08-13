/*
 * Client.cpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#include "client/Client.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "input/KeyboardHandler.hpp"
#include "input/MouseHandler.hpp"

#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"
#include "scene/Camera.hpp"

#include "renderer/Renderer.hpp"
#include "renderer/DebugRenderer.hpp"
#include "renderer/ResourceManager.hpp"
#include "renderer/BufferedShader.hpp"

#include "client/Interface.hpp"
#include "client/CameraController.hpp"
#include "client/Interface.hpp"

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Shader.hpp"
#include "assets/FileWatcher.hpp"

#include "threading/ThreadPool.hpp"

#include "physics/Octree.hpp"

#include "Config.hpp"
#include "Utilities.hpp"

Client::Client(int argc, char** argv)
    : keyboardHandler(), mouseHandler()
    , scene()
    , interface()
    , cameraController()
    , assetManager()
    , renderer(), debugRenderer()
    , threadPool()

    , sdlWindow(nullptr), sdlRenderer(nullptr)
    , timeFrameBegin(), timeFrameEnd()
    , frameCount(0)
    , timeFrameFPS()
    , frameCountFPS(0)

{
    glutInit(&argc, argv);
}

void Client::initialize(SDL_Window* sdlWindow, SDL_Renderer* sdlRenderer)
{
    cleanup();

    this->sdlWindow = sdlWindow;
    this->sdlRenderer = sdlRenderer;

    keyboardHandler = std::make_unique<input::KeyboardHandler>(shared_from_this());
    mouseHandler = std::make_unique<input::MouseHandler>(shared_from_this());
    interface = std::make_unique<Interface>(shared_from_this());
    cameraController = std::make_unique<CameraController>(shared_from_this());

    scene = std::make_unique<scene::Scene>();
    assetManager = std::make_unique<assets::AssetManager>();
    renderer = std::make_unique<renderer::Renderer>();
    debugRenderer = std::make_unique<renderer::DebugRenderer>();
    threadPool = std::make_unique<threading::ThreadPool>();

    threadPool->settings.numThreads = SDL_GetCPUCount() - 1;
    threadPool->initialize();

    interface->initialize();
    renderer->initialize();
    debugRenderer->initialize();

    scene->camera->position = glm::vec3(-14.0f, 1.5f, 0.0f);
    scene->camera->rotate(90.0f, 30.0f);

    scene->root->models.push_back(assetManager->getOrCreate<assets::Model>("models/crytek-sponza/sponza.obj", std::ref(*assetManager)));

//    auto child = std::make_unique<scene::SceneNode>();
//    child->model = assetManager->getOrCreate<assets::Model>("models/shuttle.obj", std::ref(*assetManager));
//    child->transformation = glm::translate(child->transformation, glm::vec3(0.0f, 0.0f, -50.0f));

    //scene->root->children.push_back(std::move(child));

    auto shader = assetManager->create<assets::Shader>("shaders/default", "shaders/default.vert", "shaders/default.frag");

    renderer->shaderHash = shader->hash;
    renderer->resourceManager->getByAsset<renderer::BufferedShader>(shader);

    scene->initialize();
}

void Client::reshape(Uint32 width, Uint32 height)
{
    auto& rendererSettings = renderer->settings;

    rendererSettings.width = width;
    rendererSettings.height = height;

    auto& debugRendererSettings = debugRenderer->settings;

    debugRendererSettings.width = width;
    debugRendererSettings.height = height;

    auto& interfaceSettings = interface->settings;

    interfaceSettings.width = width;
    interfaceSettings.height = height;

    interface->initialize();
}

void Client::prepareFrame()
{
    ++frameCount;
    ++frameCountFPS;

    const auto now = timeFrameBegin = std::chrono::high_resolution_clock::now();
    const auto msSinceLastFrame = std::chrono::duration_cast<std::chrono::milliseconds>(now - timeFrameEnd).count();
    const auto msSinceLastFPS = std::chrono::duration_cast<std::chrono::milliseconds>((now - timeFrameFPS)).count();

    UNUSED(msSinceLastFrame);

    if (msSinceLastFPS > 1000)
    {
        const unsigned int framesPerSecond = (frameCountFPS * 1000) / (msSinceLastFPS);

        frameCountFPS = 0;
        timeFrameFPS = now;

        interface->data.fps = framesPerSecond;
    }

    cameraController->update(std::chrono::duration_cast<std::chrono::microseconds>(now - timeFrameEnd).count());
    assetManager->fileWatcher->update();

//    scene->root->transformation =
//            glm::rotate(scene->root->transformation, 1.0f, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
//
//    scene->camera->rotate(-1.0f, 0.0f);
//
//    scene->root->children[0]->transformation =
//            glm::rotate(scene->root->children[0]->transformation, 1.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
//
//    scene->root->children[1]->transformation =
//            glm::rotate(scene->root->children[1]->transformation, 1.0f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

    if (1)
    {
        std::vector<threading::Task> tasks;

        for (unsigned int i = 0; i < 100; ++i)
        {
            tasks.emplace_back([=](){ for(unsigned int i = 0; i < 1000000; ++i); });
        }

        threadPool->enqueue(tasks);
    }

    threadPool->synchronize(threading::Task::Flags::ALL_FLAGS);

    assert(threadPool->getNumTasks() == 0);
}

void Client::finalizeFrame()
{
    renderer::Renderer::RenderResults results;
    renderer->render(*scene, results);

    debugRenderer->render(*scene);

    interface->data.cameraPosition = scene->camera->position;
    interface->data.cameraDirection = scene->camera->forward();

    interface->data.renderTime = results.renderTime.count();
    interface->display(sdlRenderer);

    timeFrameEnd = std::chrono::high_resolution_clock::now();
}

void Client::event(SDL_Event* event)
{
    if (nullptr == event)
        return;

    switch (event->type)
    {
    case SDL_KEYDOWN:
        if (SDLK_ESCAPE == event->key.keysym.sym)
        {
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;

            SDL_PushEvent(&quitEvent);
        } else
        {
            keyboardHandler->event(event->key);
        }
        break;
    case SDL_KEYUP:
        keyboardHandler->event(event->key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mouseHandler->event(event->button);
        break;
    case SDL_MOUSEBUTTONUP:
        mouseHandler->event(event->button);
        break;
    case SDL_MOUSEMOTION:
        mouseHandler->event(event->motion);
        break;
    default:
        break;
    }
}

void Client::cleanup()
{
    if (threadPool)
        threadPool->synchronize(threading::Task::Flags::ALL_FLAGS);

    keyboardHandler.reset();
    mouseHandler.reset();
    scene.reset();
    interface.reset();
    cameraController.reset();
    renderer.reset();
    threadPool.reset();
}

