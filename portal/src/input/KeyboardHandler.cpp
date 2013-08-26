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
#include "client/Interface.hpp"

#include "scene/Scene.hpp"

#include "renderer/Renderer.hpp"
#include "renderer/DebugRenderer.hpp"

#include "assets/AssetManager.hpp"

#include "Utilities.hpp"

input::KeyboardHandler::KeyboardHandler(std::shared_ptr<Client> client)
    : client(client)
{
    {
        keydownMap[KeyModPair{'1', 0}] = [] (Client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::FULL); };
        keydownMap[KeyModPair{'2', 0}] = [] (Client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::DEPTH_ONLY); };
        keydownMap[KeyModPair{'3', 0}] = [] (Client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::NORMALS_ONLY); };
        keydownMap[KeyModPair{'4', 0}] = [] (Client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::ALBEDO_ONLY); };
        keydownMap[KeyModPair{'5', 0}] = [] (Client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::AMBIENT_OCCLUSION_ONLY); };
        keydownMap[KeyModPair{'6', 0}] = [] (Client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::POSITIONS_ONLY); };
    }

    keydownMap[KeyModPair{'c', 0}] = [] (Client& c) { c.debugRenderer->lines.clear(); };
    keydownMap[KeyModPair{'q', 0}] = [] (Client& c) { c.assetManager->reportCacheContents(); };
    keydownMap[KeyModPair{'v', 0}] = [] (Client& c) { c.renderer->settings.toggleAmbientOcclusion(); };
    keydownMap[KeyModPair{'b', 0}] = [] (Client& c) { c.renderer->settings.toggleBumpMapping(); };
    keydownMap[KeyModPair{'n', 0}] = [] (Client& c) { c.renderer->settings.toggleLighting(); };
    keydownMap[KeyModPair{'t', 0}] = [] (Client& c) { c.interface->toggleConsole(); };

    keydownMap[KeyModPair{SDLK_UP, 0}] = [] (Client& c) { c.interface->scroll(1);  };
    keydownMap[KeyModPair{SDLK_DOWN, 0}] = [] (Client& c) { c.interface->scroll(-1);  };

    keydownMap[KeyModPair{SDLK_PAGEUP, 0}] = [] (Client& c) { c.interface->scroll(c.interface->console.linesPerPage - 1);  };
    keydownMap[KeyModPair{SDLK_PAGEDOWN, 0}] = [] (Client& c) { c.interface->scroll(-c.interface->console.linesPerPage + 1);  };

    keydownMap[KeyModPair{SDLK_HOME, 0}] = [] (Client& c) { c.interface->scrollTo(-1);  };
    keydownMap[KeyModPair{SDLK_END, 0}] = [] (Client& c) { c.interface->scrollTo(0);  };

    keydownMap[KeyModPair{'1', KMOD_LSHIFT}] = [] (Client& c) { c.debugRenderer->settings.drawBVH = !c.debugRenderer->settings.drawBVH; };
    keydownMap[KeyModPair{'2', KMOD_LSHIFT}] = [] (Client& c) { c.debugRenderer->settings.drawRays = !c.debugRenderer->settings.drawRays; };
    keydownMap[KeyModPair{'3', KMOD_LSHIFT}] = [] (Client& c) { c.debugRenderer->settings.drawCrosshair = !c.debugRenderer->settings.drawCrosshair; };

    keydownMap[KeyModPair{SDLK_ESCAPE, 0}] = [] (Client& c) { UNUSED(c); SDL_Log("ESC pressed, exit inevitable"); };
}

input::KeyboardHandler::~KeyboardHandler()
{
}

void input::KeyboardHandler::event(const SDL_KeyboardEvent& event) const
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        keydown(event.keysym.sym, event.keysym.mod); break;
    case SDL_KEYUP:
        keyup(event.keysym.sym, event.keysym.mod); break;
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
    callKeyFunction(keydownMap, key, mod, *client);

    client->cameraController->keyDown(key, mod);
}

void input::KeyboardHandler::keyup(SDL_Keycode key, Uint16 mod) const
{
    client->cameraController->keyUp(key, mod);

    callKeyFunction(keyupMap, key, mod, *client);
}


