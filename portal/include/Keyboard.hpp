/*
 * Keyboard.hpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include "Client.hpp"

#include <SDL/SDL.h>

class Keyboard
{
public:
    void event(const SDL_KeyboardEvent& event);

    void keydown(SDLKey key, SDLMod mod);
    void keyup(SDLKey key, SDLMod mod);

    void setClient(const std::shared_ptr<Client>& client)
    {
        this->client = client;
    }
private:
    std::shared_ptr<Client> client;
};


#endif /* KEYBOARD_HPP_ */
