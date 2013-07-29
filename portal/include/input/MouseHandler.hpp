/*
 * Keyboard.hpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#ifndef MOUSEHANDLER_HPP_
#define MOUSEHANDLER_HPP_

#include <SDL2/SDL.h>

#include "Client.hpp"
#include "CommandSet.hpp"

class MouseHandler
{
public:
    void event(const SDL_MouseButtonEvent& event);
    void event(const SDL_MouseMotionEvent& event);

    void mouseDown(Uint8 button, Uint16 x, Uint16 y);
    void mouseUp(Uint8 button, Uint16 x, Uint16 y);
    void mouseMove(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel);

    std::shared_ptr<Client> client;
    std::shared_ptr<CommandSet> commandSet;
};



#endif /* MOUSEHANDLER_HPP_ */
