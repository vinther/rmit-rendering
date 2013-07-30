/*
 * KeyboardHandler.cpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#include "input/KeyboardHandler.hpp"

#include "client/Client.hpp"
#include "client/CameraController.hpp"
#include "scene/Scene.hpp"
#include "assets/AssetManager.hpp"

KeyboardHandler::KeyboardHandler(std::shared_ptr<Client> client)
    : client(client)
{
}

void KeyboardHandler::event(const SDL_KeyboardEvent& event)
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

void KeyboardHandler::keydown(SDL_Keycode key, Uint16 mod)
{
    if (key == SDLK_ESCAPE)
        SDL_Log("ESC pressed, exit inevitable");

    if ('q' == key)
        client->scene->assetManager->reportCacheContents();

    client->cameraController->keyDown(key, mod);
}

void KeyboardHandler::keyup(SDL_Keycode key, Uint16 mod)
{
    client->cameraController->keyUp(key, mod);
}


