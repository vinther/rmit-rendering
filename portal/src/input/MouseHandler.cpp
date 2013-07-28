/*
 * KeyboardHandler.cpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#include "input/MouseHandler.hpp"

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

void MouseHandler::mouseMove(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel)
{
}
