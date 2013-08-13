/*
 * CameraController.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "client/CameraController.hpp"

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
        state.moveSpeed = 2.0f; break;
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
        state.moveSpeed = 1.0f; break;
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

    client->scene->camera->rotate(xrel * 0.01f, yrel * 0.01f);
}

void CameraController::update(Uint32 microseconds)
{
    auto& camera = *(client->scene->camera);

    const float f = (float) microseconds * 0.001f;

    if (state.moveForward)
        camera.position += camera.forward() * state.moveSpeed * f;

    if (state.moveBackward)
        camera.position += -camera.forward() * state.moveSpeed * f;

    if (state.moveRight)
        camera.position += camera.right() * state.moveSpeed * f;

    if (state.moveLeft)
        camera.position += -camera.right() * state.moveSpeed * f;

    if (state.moveUp)
        camera.position += camera.up() * state.moveSpeed * f;

    if (state.moveDown)
        camera.position += -camera.up() * state.moveSpeed * f;
}

CameraController::State::State()
    : moveForward(false), moveBackward(false), moveLeft(false)
    , moveRight(false), moveUp(false), moveDown(false)
    , moveSpeed(1.0f)
{
}
