/*
 * Client.cpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#include "client/Client.hpp"

#include "input/KeyboardHandler.hpp"
#include "input/MouseHandler.hpp"

#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"
#include "scene/Camera.hpp"

#include "renderer/Renderer.hpp"

#include "client/Interface.hpp"
#include "client/CameraController.hpp"
#include "client/Interface.hpp"

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Shader.hpp"
#include "assets/FileWatcher.hpp"

#include "Utilities.hpp"

Client::Client(int argc, char** argv)
    : keyboardHandler()
    , mouseHandler()
    , scene()
    , interface()
    , cameraController()
    , renderer()
{
    glutInit(&argc, argv);
}

void Client::initialize()
{
    keyboardHandler = std::make_unique<KeyboardHandler>(shared_from_this());
    mouseHandler = std::make_unique<MouseHandler>(shared_from_this());
    scene = std::make_unique<Scene>(shared_from_this());
    interface = std::make_unique<Interface>(shared_from_this());
    cameraController = std::make_unique<CameraController>(shared_from_this());
    renderer = std::make_unique<Renderer>();
    renderer->initialize();

    scene->camera->position = glm::vec3(0.0f, 0.0f, 20.0f);

    scene->assetManager->create<Shader>("shaders/helloWorld", "shaders/helloWorld.vert", "shaders/helloWorld.frag");
}

void Client::reshape(Uint32 width, Uint32 height)
{
    auto& rendererSettings = renderer->settings;

    rendererSettings.width = width;
    rendererSettings.height = height;

    renderer->initialize();
}


#include <glm/gtc/matrix_transform.hpp>

void Client::update(Uint32 ms)
{
    cameraController->update(ms);
    scene->assetManager->fileWatcher->update();

//    //rot += 0.01f;
//    scene->root->transformation =
//            glm::rotate(scene->root->transformation, 1.0f, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
//
//    scene->root->children[0]->transformation =
//            glm::rotate(scene->root->children[0]->transformation, 1.0f, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
//
//    scene->root->children[1]->transformation =
//            glm::rotate(scene->root->children[1]->transformation, 1.0f, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

    interface->data.camPos = scene->camera->position;
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
        }

        keyboardHandler->event(event->key);
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


void Client::display()
{
    Renderer::RenderResults results;
    renderer->render(*scene, results);

    interface->data.renderTime = results.renderTime.count();
    interface->display();
}

void Client::cleanup()
{
    keyboardHandler.reset();
    mouseHandler.reset();
    scene.reset();
    interface.reset();
    cameraController.reset();
    renderer.reset();
}

