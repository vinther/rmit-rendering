/*
 * Keyboard.hpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#ifndef MOUSEHANDLER_HPP_
#define MOUSEHANDLER_HPP_

#include <memory>

#include <SDL2/SDL.h>

class Client;

struct SDL_MouseButtonEvent;
struct SDL_MouseMotionEvent;

class MouseHandler
{
public:
    MouseHandler(std::shared_ptr<Client> client);

    void event(const SDL_MouseButtonEvent& event);
    void event(const SDL_MouseMotionEvent& event);

    void mouseDown(Uint8 button, Uint16 x, Uint16 y);
    void mouseUp(Uint8 button, Uint16 x, Uint16 y);
    void mouseMove(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel);

    std::shared_ptr<Client> client;
};



#endif /* MOUSEHANDLER_HPP_ */
