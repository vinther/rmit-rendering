/*
 * CameraController.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "client/CameraController.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "client/Client.hpp"
#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

#include "Utilities.hpp"

CameraController::CameraController(std::shared_ptr<Client> client)
    : state()
    , client(client)
{
}

CameraController::~CameraController()
{
    // TODO Auto-generated destructor stub
}

void CameraController::keyDown(SDL_Keycode key, Uint16 mod)
{
    UNUSED(mod);

    switch (key)
    {
    case 'w':
        state.moveForward = true; break;
    case 's':
        state.moveBackward = true; break;
    case 'd':
        state.moveRight = true; break;
    case 'a':
        state.moveLeft = true; break;
    case SDLK_SPACE:
        state.moveUp = true; break;
    case SDLK_LCTRL:
        state.moveDown = true; break;
    case SDLK_LSHIFT:
        state.speedFactor = 16.0f; break;
    default:
        break;
    }
}

void CameraController::keyUp(SDL_Keycode key, Uint16 mod)
{
    UNUSED(mod);

    switch (key)
    {
    case 'w':
        state.moveForward = false; break;
    case 's':
        state.moveBackward = false; break;
    case 'd':
        state.moveRight = false; break;
    case 'a':
        state.moveLeft = false; break;
    case SDLK_SPACE:
        state.moveUp = false; break;
    case SDLK_LCTRL:
        state.moveDown = false; break;
    case SDLK_LSHIFT:
        state.speedFactor = 8.0f; break;
    default:
        break;
    }
}

void CameraController::mouseDown(Uint8 button, Uint16 x, Uint16 y)
{
    UNUSED(button);
    UNUSED(x);
    UNUSED(y);
}

void CameraController::mouseUp(Uint8 button, Uint16 x, Uint16 y)
{
    UNUSED(button);
    UNUSED(x);
    UNUSED(y);
}

void CameraController::mouseMove(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel)
{
    UNUSED(x);
    UNUSED(y);

    auto& rotation = client->scene->camera->state.rotation;

    rotation = rotation * (glm::quat(1.0f, glm::vec3(0.0f, xrel * 0.0001f, 0.0f)));
    rotation = (glm::quat(1.0f, glm::vec3(yrel * 0.0001f, 0.0f, 0.0f))) * rotation;
    rotation = glm::normalize(rotation);
}

void CameraController::update(Uint32 ms)
{
    UNUSED(ms);

    auto& position = client->scene->camera->state.position;
    const auto& rotation = client->scene->camera->state.rotation;

    if (state.moveForward)
        position += glm::vec3(0.0f, 0.0f, -state.speedFactor) * rotation;

    if (state.moveBackward)
        position += glm::vec3(0.0f, 0.0f, state.speedFactor)  * rotation;

    if (state.moveRight)
        position += glm::vec3(state.speedFactor, 0.0f,  0.0f) * rotation;

    if (state.moveLeft)
        position += glm::vec3(-state.speedFactor, 0.0f,  0.0f) * rotation;

    if (state.moveUp)
        position += glm::vec3(0.0f, state.speedFactor,  0.0f);// * rotation;

    if (state.moveDown)
        position += glm::vec3(0.0f, -state.speedFactor,  0.0f);// * rotation;
}

CameraController::State::State()
    : moveForward(false), moveBackward(false), moveLeft(false)
    , moveRight(false), moveUp(false), moveDown(false)
    , speedFactor(8.0f)
{
}
