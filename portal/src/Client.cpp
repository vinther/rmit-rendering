/*
 * Client.cpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#include "Client.hpp"

#include "input/KeyboardHandler.hpp"
#include "input/MouseHandler.hpp"

#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

#include "Interface.hpp"
#include "CameraController.hpp"
#include "Utilities.hpp"
#include "Interface.hpp"
#include "Renderer.hpp"

#include "assets/AssetManager.hpp"

Client::Client(int argc, char** argv)
{
    // TODO Auto-generated constructor stub

}

Client::~Client()
{
    // TODO Auto-generated destructor stub
}

void Client::initialize()
{
    char fakeParam[] = "";
    char *fakeargv[] = {fakeParam};
    int argc = 0;

    glutInit( &argc, fakeargv );

    keyboardHandler = std::make_unique<KeyboardHandler>(shared_from_this());
    mouseHandler = std::make_unique<MouseHandler>(shared_from_this());
    scene = std::make_unique<Scene>(shared_from_this());
    interface = std::make_unique<Interface>(shared_from_this());
    cameraController = std::make_unique<CameraController>(shared_from_this());
    renderer = std::make_unique<Renderer>();

    scene->camera->position = glm::vec3(0.0f, 0.0f, 20.0f);

    scene->assetManager->get<Asset>("models/shuttle2.obj");
}

void Client::reshape(Uint32 width, Uint32 height)
{
    auto& rendererSettings = renderer->settings;

    rendererSettings.width = width;
    rendererSettings.height = height;

    renderer->initialize();
}

void Client::update(Uint32 ms)
{
    cameraController->update(ms);
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


void Client::display(SDL_Surface* surface)
{
    Renderer::RenderResults results;
    renderer->render(*scene, results);

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

