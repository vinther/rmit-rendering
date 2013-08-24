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
#include "renderer/resources/Shader.hpp"

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

struct Globals
{
    std::shared_ptr<Client> client;
};

static Globals globals;

static void globalLogFunction(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    if (globals.client)
        globals.client->logFunction(userdata, category, priority, message);
}

Client::Client(int argc, char** argv)
    : keyboardHandler(), mouseHandler()
    , scene()
    , interface()
    , cameraController()
    , assetManager()
    , renderer(), debugRenderer()
    , threadPool()
    , timeFrameBegin(), timeFrameEnd()
    , frameCount(0)
    , timeFrameFPS()
    , frameCountFPS(0)
    , defaultLogOutputFunction(nullptr)
{
    glutInit(&argc, argv);
}

void Client::initialize(SDL_Window* window, SDL_GLContext context)
{
    cleanup();

    globals.client = shared_from_this();

    void* defaultUserData;

    SDL_LogGetOutputFunction(&defaultLogOutputFunction, &defaultUserData);

    logFunction = [&](void *userdata, int category, SDL_LogPriority priority, const char *message) {
        defaultLogOutputFunction(userdata, category, priority, message);
        interface->addMessage(category, priority, message);
    };

    interface = std::make_unique<Interface>(shared_from_this());

    SDL_LogSetOutputFunction(globalLogFunction, nullptr);
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Initializing client");

    keyboardHandler = std::make_unique<input::KeyboardHandler>(shared_from_this());
    mouseHandler = std::make_unique<input::MouseHandler>(shared_from_this());
    cameraController = std::make_unique<CameraController>(shared_from_this());
    scene = std::make_unique<scene::Scene>();
    assetManager = std::make_unique<assets::AssetManager>();
    renderer = std::make_unique<renderer::Renderer>();
    debugRenderer = std::make_unique<renderer::DebugRenderer>();
    threadPool = std::make_unique<threading::ThreadPool>();

    threadPool->settings.numThreads = SDL_GetCPUCount() - 1;
    threadPool->initialize();
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Threadpool initialized");

    interface->initialize();
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Interface initialized");

    renderer->initialize(window, context, *(assetManager));
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Renderer initialized");

    debugRenderer->initialize();
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Debug initialized");

    scene->root->models.push_back(assetManager->getOrCreate<assets::Model>("models/crytek-sponza/sponza.obj", std::ref(*assetManager)));
    scene->initialize();
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Scene initialized");

    scene->camera->position = glm::vec3(-14.0f, 1.5f, 0.0f);
    scene->camera->rotate(90.0f, 30.0f);
}

void Client::reshape(Uint32 width, Uint32 height)
{
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Window reshaped to %dx%d", width, height);

	scene->camera->aspectRatio = (float) (width) / (float) height;

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

void Client::prepareFrame(SDL_Window* window, SDL_GLContext context)
{
    ++frameCount;
    ++frameCountFPS;

    const auto now = timeFrameBegin = std::chrono::high_resolution_clock::now();
    const auto msSinceLastFPS = std::chrono::duration_cast<std::chrono::milliseconds>((now - timeFrameFPS)).count();
    const auto microsSinceLastFrame = std::chrono::duration_cast<std::chrono::microseconds>(now - timeFrameEnd).count();

    if (msSinceLastFPS > 1000)
    {
        const unsigned int framesPerSecond = (frameCountFPS * 1000) / (msSinceLastFPS);

        frameCountFPS = 0;
        timeFrameFPS = now;

        interface->data.fps = framesPerSecond;
    }

    cameraController->update(microsSinceLastFrame);
    interface->update(microsSinceLastFrame);
    assetManager->fileWatcher->update();

    renderer->prepareFrame(*(threadPool), window, context);

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

//    if (1)
//    {
//        std::vector<threading::Task> tasks;
//
//        for (unsigned int i = 0; i < 100; ++i)
//        {
//            tasks.emplace_back([=](){ for(unsigned int i = 0; i < 100000; ++i); });
//        }
//
//        threadPool->enqueue(tasks);
//    }

    threadPool->synchronize(threading::Task::Flags::ALL_FLAGS);

    assert(threadPool->getNumTasks() == 0);
}

void Client::finalizeFrame(SDL_Window* window, SDL_GLContext context)
{
	UNUSED(window);
	UNUSED(context);

    renderer::Renderer::RenderResults results;
    renderer->render(*scene, results);

    debugRenderer->render(*scene);

    interface->data.cameraPosition = scene->camera->position;
    interface->data.cameraDirection = scene->camera->forward();

    Interface::RenderResults interfaceRenderResults;
    interface->data.renderTime = results.renderTime.count();
    interface->render(interfaceRenderResults);

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
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_CLIENT, "Cleaning up client resources");

    if (threadPool)
        threadPool->synchronize(threading::Task::Flags::ALL_FLAGS);

    keyboardHandler.reset();
    mouseHandler.reset();
    scene.reset();
    interface.reset();
    cameraController.reset();
    assetManager.reset();
    renderer.reset();
    debugRenderer.reset();
    threadPool.reset();

    if (this == globals.client.get())
        globals.client.reset();

    if (defaultLogOutputFunction)
        SDL_LogSetOutputFunction(defaultLogOutputFunction, nullptr);
}

