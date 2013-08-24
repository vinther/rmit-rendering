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
        using ROutput = renderer::Renderer::Settings::OutputMode;

        keydownMap[{'1', 0}] = [] (Client& c) { c.renderer->settings.setOutput(ROutput::FULL); };
        keydownMap[{'2', 0}] = [] (Client& c) { c.renderer->settings.setOutput(ROutput::DEPTH_ONLY); };
        keydownMap[{'3', 0}] = [] (Client& c) { c.renderer->settings.setOutput(ROutput::NORMALS_ONLY); };
        keydownMap[{'4', 0}] = [] (Client& c) { c.renderer->settings.setOutput(ROutput::ALBEDO_ONLY); };
        keydownMap[{'5', 0}] = [] (Client& c) { c.renderer->settings.setOutput(ROutput::AMBIENT_OCCLUSION_ONLY); };
        keydownMap[{'6', 0}] = [] (Client& c) { c.renderer->settings.setOutput(ROutput::POSITIONS_ONLY); };
    }

    keydownMap[{'c', 0}] = [] (Client& c) { c.debugRenderer->lines.clear(); };
    keydownMap[{'q', 0}] = [] (Client& c) { c.assetManager->reportCacheContents(); };
    keydownMap[{'v', 0}] = [] (Client& c) { c.renderer->settings.toggleAmbientOcclusion(); };
    keydownMap[{'b', 0}] = [] (Client& c) { c.renderer->settings.toggleBumpMapping(); };
    keydownMap[{'n', 0}] = [] (Client& c) { c.renderer->settings.toggleLighting(); };
    keydownMap[{'t', 0}] = [] (Client& c) { c.interface->toggleConsole(); };

    keydownMap[{SDLK_UP, 0}] = [] (Client& c) { c.interface->scroll(1);  };
    keydownMap[{SDLK_DOWN, 0}] = [] (Client& c) { c.interface->scroll(-1);  };

    keydownMap[{SDLK_PAGEUP, 0}] = [] (Client& c) { c.interface->scroll(c.interface->console.linesPerPage - 1);  };
    keydownMap[{SDLK_PAGEDOWN, 0}] = [] (Client& c) { c.interface->scroll(-c.interface->console.linesPerPage + 1);  };

    keydownMap[{SDLK_HOME, 0}] = [] (Client& c) { c.interface->scrollTo(-1);  };
    keydownMap[{SDLK_END, 0}] = [] (Client& c) { c.interface->scrollTo(0);  };

    keydownMap[{'1', KMOD_LSHIFT}] = [] (Client& c) { c.debugRenderer->settings.drawBVH = !c.debugRenderer->settings.drawBVH; };
    keydownMap[{'2', KMOD_LSHIFT}] = [] (Client& c) { c.debugRenderer->settings.drawRays = !c.debugRenderer->settings.drawRays; };
    keydownMap[{'3', KMOD_LSHIFT}] = [] (Client& c) { c.debugRenderer->settings.drawCrosshair = !c.debugRenderer->settings.drawCrosshair; };

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


