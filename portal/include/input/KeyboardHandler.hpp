/*
 * Keyboard.hpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#ifndef KEYBOARDHANDLER_HPP_
#define KEYBOARDHANDLER_HPP_

#include <memory>

#include <SDL2/SDL.h>

class Client;
class CommandSet;

namespace input
{

class KeyboardHandler
{
public:
    KeyboardHandler(std::shared_ptr<Client> client);
    ~KeyboardHandler();

    void event(const SDL_KeyboardEvent& event);

    void keydown(SDL_Keycode key, Uint16 mod);
    void keyup(SDL_Keycode key, Uint16 mod);
private:
    std::shared_ptr<Client> client;
};

}

#endif /* KEYBOARDHANDLER_HPP_ */
