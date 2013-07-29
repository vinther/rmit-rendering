/*
 * KeyboardHandler.cpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#include "input/MouseHandler.hpp"

#include "scene/Scene.hpp"

void MouseHandler::event(const SDL_MouseButtonEvent& event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        mouseDown(event.button, event.x, event.y);
        break;
    case SDL_MOUSEBUTTONUP:
        mouseUp(event.button, event.x, event.y);
        break;
    }
}

void MouseHandler::event(const SDL_MouseMotionEvent& event)
{
    mouseMove(event.x, event.y, event.xrel, event.yrel);
}

void MouseHandler::mouseDown(Uint8 button, Uint16 x, Uint16 y)
{
//    switch (button)
//    {
//    case SDLKE
//    }

    std::cout << (int) button << std::endl;
}

void MouseHandler::mouseUp(Uint8 button, Uint16 x, Uint16 y)
{
}

Uint16 px, py;

void MouseHandler::mouseMove(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel)
{

    auto& rotation = client->scene->camera->rotation;

    Sint16 dx = xrel;
    Sint16 dy = yrel;
    std::cout << dx << " " << dy << " " << x << " " << y << std::endl;

    rotation = rotation * (glm::quat(1.0f, glm::vec3(0.0f, dx * 0.0001f, 0.0f)));
    rotation = (glm::quat(1.0f, glm::vec3(dy * 0.0001f, dx * 0.000f, 0.0f))) * rotation;
    rotation = glm::normalize(rotation);

    px = x;
    py = y;
}
