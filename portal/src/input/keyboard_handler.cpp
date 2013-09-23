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

#include "assets/DataStore.hpp"

#include "shared/utilities.hpp"

input::keyboard_handler::keyboard_handler(std::shared_ptr<client> client)
    : client(client)
{
    {
        keydownMap[KeyModPair{'1', 0}] = [] (client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::FULL); };
        keydownMap[KeyModPair{'2', 0}] = [] (client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::DEPTH_ONLY); };
        keydownMap[KeyModPair{'3', 0}] = [] (client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::NORMALS_ONLY); };
        keydownMap[KeyModPair{'4', 0}] = [] (client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::ALBEDO_ONLY); };
        keydownMap[KeyModPair{'5', 0}] = [] (client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::AMBIENT_OCCLUSION_ONLY); };
        keydownMap[KeyModPair{'6', 0}] = [] (client& c) { c.renderer->settings.setOutput(renderer::Renderer::Settings::POSITIONS_ONLY); };
    }

    keydownMap[KeyModPair{'c', 0}] = [] (client& c) { c.debugRenderer->lines.clear(); };
    keydownMap[KeyModPair{'q', 0}] = [] (client& c) { c.assetManager->reportCacheContents(); };
    keydownMap[KeyModPair{'v', 0}] = [] (client& c) { c.renderer->settings.toggleAmbientOcclusion(); };
    keydownMap[KeyModPair{'b', 0}] = [] (client& c) { c.renderer->settings.toggleBumpMapping(); };
    keydownMap[KeyModPair{'n', 0}] = [] (client& c) { c.renderer->settings.toggleLighting(); };
    keydownMap[KeyModPair{'t', 0}] = [] (client& c) { c.interface->toggleConsole(); };

    keydownMap[KeyModPair{SDLK_UP, 0}] = [] (client& c) { c.interface->scroll(1);  };
    keydownMap[KeyModPair{SDLK_DOWN, 0}] = [] (client& c) { c.interface->scroll(-1);  };

    keydownMap[KeyModPair{SDLK_PAGEUP, 0}] = [] (client& c) { c.interface->scroll(c.interface->console.linesPerPage - 1);  };
    keydownMap[KeyModPair{SDLK_PAGEDOWN, 0}] = [] (client& c) { c.interface->scroll(-c.interface->console.linesPerPage + 1);  };

    keydownMap[KeyModPair{SDLK_HOME, 0}] = [] (client& c) { c.interface->scrollTo(-1);  };
    keydownMap[KeyModPair{SDLK_END, 0}] = [] (client& c) { c.interface->scrollTo(0);  };

    keydownMap[KeyModPair{'1', KMOD_LSHIFT}] = [] (client& c) { c.debugRenderer->settings.drawBVH = !c.debugRenderer->settings.drawBVH; };
    keydownMap[KeyModPair{'2', KMOD_LSHIFT}] = [] (client& c) { c.debugRenderer->settings.drawRays = !c.debugRenderer->settings.drawRays; };
    keydownMap[KeyModPair{'3', KMOD_LSHIFT}] = [] (client& c) { c.debugRenderer->settings.drawCrosshair = !c.debugRenderer->settings.drawCrosshair; };

    keydownMap[KeyModPair{SDLK_ESCAPE, 0}] = [] (client& c) { UNUSED(c); SDL_Log("ESC pressed, exit inevitable"); };
}

input::keyboard_handler::~keyboard_handler()
{
}

void input::keyboard_handler::handle_event(const SDL_KeyboardEvent& event) const
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        inject_key_down(event.keysym.sym, event.keysym.mod); break;
    case SDL_KEYUP:
        inject_key_up(event.keysym.sym, event.keysym.mod); break;
    default:
        break;
    }
}

template <class T>
void invoke_key_function(const T& keymap, Uint16 key, Uint16 mod, client& client)
{
    const auto& function = keymap.find({key, mod});

    if (function != std::end(keymap))
    {
        function->second(client);
    }
}

void input::keyboard_handler::inject_key_down(SDL_Keycode key, Uint16 mod) const
{
    invoke_key_function(keydownMap, key, mod, *client);

    client->cameraController->keyDown(key, mod);
}

void input::keyboard_handler::inject_key_up(SDL_Keycode key, Uint16 mod) const
{
    client->cameraController->keyUp(key, mod);

    invoke_key_function(keyupMap, key, mod, *client);
}


