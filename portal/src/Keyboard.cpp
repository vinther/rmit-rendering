/*
 * Keyboard.cpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#include "Keyboard.hpp"

void Keyboard::event(const SDL_KeyboardEvent& event)
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

void Keyboard::keydown(SDLKey key, SDLMod mod)
{
    if (commandSet)
        commandSet->keydown(key, mod);
}

void Keyboard::keyup(SDLKey key, SDLMod mod)
{
    if (commandSet)
        commandSet->keyup(key, mod);
}
