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

#include "renderer/Renderer.hpp"
#include "renderer/DebugRenderer.hpp"

#include "assets/AssetManager.hpp"

#include "Utilities.hpp"

input::KeyboardHandler::KeyboardHandler(std::shared_ptr<Client> client)
    : client(client)
{

    {
        using ROutput = renderer::Renderer::Settings::Output;

        keydownMap[{'1', 0}] = [] (Client& c) { c.renderer->settings.output = ROutput::OUTPUT_FULL; };
        keydownMap[{'2', 0}] = [] (Client& c) { c.renderer->settings.output = ROutput::OUTPUT_DEPTH; };
        keydownMap[{'3', 0}] = [] (Client& c) { c.renderer->settings.output = ROutput::OUTPUT_NORMALS; };
        keydownMap[{'4', 0}] = [] (Client& c) { c.renderer->settings.output = ROutput::OUTPUT_ALBEDO; };
        keydownMap[{'5', 0}] = [] (Client& c) { c.renderer->settings.output = ROutput::OUTPUT_POSITIONS; };
        keydownMap[{'6', 0}] = [] (Client& c) { c.renderer->settings.output = ROutput::OUTPUT_AMBIENT_OCCLUSION; };
        keydownMap[{'7', 0}] = [] (Client& c) { c.renderer->settings.output = ROutput::OUTPUT_FULL; };
        keydownMap[{'8', 0}] = [] (Client& c) { c.renderer->settings.bumpMapping = !c.renderer->settings.bumpMapping; };
    }

    keydownMap[{'c', 0}] = [] (Client& c) { c.debugRenderer->lines.clear(); };
    keydownMap[{'q', 0}] = [] (Client& c) { c.assetManager->reportCacheContents(); };
    keydownMap[{SDLK_ESCAPE, 0}] = [] (Client& c) { UNUSED(c); SDL_Log("ESC pressed, exit inevitable"); };
}

input::KeyboardHandler::~KeyboardHandler()
{
}

void input::KeyboardHandler::event(const SDL_KeyboardEvent& event) const
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

template <class T>
void callKeyFunction(const T& keymap, Uint16 key, Uint16 mod, Client& client)
{
    const auto& keyFunction = keymap.find({key, mod});

    if (keyFunction != std::end(keymap))
    {
        keyFunction->second(client);
    }
}

void input::KeyboardHandler::keydown(SDL_Keycode key, Uint16 mod) const
{
    if ('1' == key)
    {
        if (mod & (KMOD_LSHIFT | KMOD_RSHIFT))
            client->debugRenderer->settings.drawBVH = !client->debugRenderer->settings.drawBVH;
    }

    if ('2' == key)
    {
        if (mod & (KMOD_LSHIFT | KMOD_RSHIFT))
            client->debugRenderer->settings.drawRays = !client->debugRenderer->settings.drawRays;

    }

    if ('3' == key)
    {
        if (mod & (KMOD_LSHIFT | KMOD_RSHIFT))
            client->debugRenderer->settings.drawCrosshair = !client->debugRenderer->settings.drawCrosshair;
    }

    callKeyFunction(keydownMap, key, mod, *client);

    client->cameraController->keyDown(key, mod);
}

void input::KeyboardHandler::keyup(SDL_Keycode key, Uint16 mod) const
{
    client->cameraController->keyUp(key, mod);

    callKeyFunction(keyupMap, key, mod, *client);
}


