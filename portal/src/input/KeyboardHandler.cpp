/*
 * KeyboardHandler.cpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#include "input/KeyboardHandler.hpp"

#include <glm/glm.hpp>

#include "client/Client.hpp"
#include "client/CameraController.hpp"

#include "scene/Scene.hpp"

#include "renderer/DebugRenderer.hpp"

#include "assets/AssetManager.hpp"

input::KeyboardHandler::KeyboardHandler(std::shared_ptr<Client> client)
    : client(client)
{
}

input::KeyboardHandler::~KeyboardHandler()
{
}

void input::KeyboardHandler::event(const SDL_KeyboardEvent& event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        keydown(event.keysym.sym, event.keysym.mod);
        break;
    case SDL_KEYUP:
        keyup(event.keysym.sym, event.keysym.mod);
        break;
    default:
        break;
    }
}

void input::KeyboardHandler::keydown(SDL_Keycode key, Uint16 mod)
{
    if (key == SDLK_ESCAPE)
        SDL_Log("ESC pressed, exit inevitable");

    if ('q' == key)
        client->assetManager->reportCacheContents();

    if ('1' == key)
        client->debugRenderer->settings.enabled = !client->debugRenderer->settings.enabled;

    if ('j' == key)
        client->debugRenderer->settings.maxTreeDepth += 1;

    if ('k' == key)
    {
        client->debugRenderer->settings.maxTreeDepth -= 1;
        client->debugRenderer->settings.maxTreeDepth = glm::max(1u, client->debugRenderer->settings.maxTreeDepth);
    }

    if ('c' == key)
    {
        client->debugRenderer->points.clear();
    }

    client->cameraController->keyDown(key, mod);
}

void input::KeyboardHandler::keyup(SDL_Keycode key, Uint16 mod)
{
    client->cameraController->keyUp(key, mod);
}


