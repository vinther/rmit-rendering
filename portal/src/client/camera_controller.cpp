/*
 * CameraController.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "client/camera_controller.hpp"

#include "client/client.hpp"
#include "scene/scene_graph.hpp"
#include "scene/camera.hpp"

#include "shared/utilities.hpp"

camera_controller::camera_controller(std::shared_ptr<client> client)
    : state()
    , client(client)
{
}

camera_controller::~camera_controller()
{
    // TODO Auto-generated destructor stub
}

void camera_controller::on_key_down(SDL_Keycode key, Uint16 mod)
{
    UNUSED(mod);

    switch (key)
    {
    case 'w':
        state.move_forward = true; break;
    case 's':
        state.move_backward = true; break;
    case 'd':
        state.move_right = true; break;
    case 'a':
        state.move_left = true; break;
    case SDLK_SPACE:
        state.move_up = true; break;
    case SDLK_LCTRL:
        state.move_down = true; break;
    case SDLK_LSHIFT:
        state.move_speed = 2.0f; break;
    default:
        break;
    }
}

void camera_controller::on_key_up(SDL_Keycode key, Uint16 mod)
{
    UNUSED(mod);

    switch (key)
    {
    case 'w':
        state.move_forward = false; break;
    case 's':
        state.move_backward = false; break;
    case 'd':
        state.move_right = false; break;
    case 'a':
        state.move_left = false; break;
    case SDLK_SPACE:
        state.move_up = false; break;
    case SDLK_LCTRL:
        state.move_down = false; break;
    case SDLK_LSHIFT:
        state.move_speed = 1.0f; break;
    default:
        break;
    }
}

void camera_controller::on_mouse_down(Uint8 button, Uint16 x, Uint16 y)
{
    UNUSED(button);
    UNUSED(x);
    UNUSED(y);
}

void camera_controller::on_mouse_up(Uint8 button, Uint16 x, Uint16 y)
{
    UNUSED(button);
    UNUSED(x);
    UNUSED(y);
}

void camera_controller::on_mouse_move(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel)
{
    UNUSED(x);
    UNUSED(y);

    client->scene->camera->rotate(xrel * 0.01f, yrel * 0.01f);
}

void camera_controller::update(Uint32 microseconds)
{
    auto& camera = *(client->scene->camera);

    const float f = (float) microseconds * 0.001f;

    if (state.move_forward)
        camera.position += camera.forward() * state.move_speed * f;

    if (state.move_backward)
        camera.position += -camera.forward() * state.move_speed * f;

    if (state.move_right)
        camera.position += camera.right() * state.move_speed * f;

    if (state.move_left)
        camera.position += -camera.right() * state.move_speed * f;

    if (state.move_up)
        camera.position += glm::vec3(0.0f, 1.0f, 0.0f) * state.move_speed * f;

    if (state.move_down)
        camera.position += -glm::vec3(0.0f, 1.0f, 0.0f) * state.move_speed * f;
}

camera_controller::State::State()
    : move_forward(false), move_backward(false), move_left(false)
    , move_right(false), move_up(false), move_down(false)
    , move_speed(1.0f)
{
}
