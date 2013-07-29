/*
 * CommandSet.hpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#ifndef COMMANDSET_HPP_
#define COMMANDSET_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>

class CommandSet
{
public:
    CommandSet(const std::shared_ptr<Client>& client)
        : client(client)
    {}

    CommandSet()
    {}

    virtual ~CommandSet()
    {}

    virtual void keydown(SDL_Keycode key, Uint16 mod) = 0;
    virtual void keyup(SDL_Keycode key, Uint16 mod) = 0;

    std::shared_ptr<Client> client;
};

#endif /* COMMANDSET_HPP_ */
