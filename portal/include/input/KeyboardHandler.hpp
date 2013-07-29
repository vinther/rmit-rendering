/*
 * Keyboard.hpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#ifndef KEYBOARDHANDLER_HPP_
#define KEYBOARDHANDLER_HPP_

#include <SDL2/SDL.h>

#include "Client.hpp"
#include "CommandSet.hpp"

class KeyboardHandler
{
public:
    void event(const SDL_KeyboardEvent& event);

    void keydown(SDL_Keycode key, Uint16 mod);
    void keyup(SDL_Keycode key, Uint16 mod);

    void setClient(const std::shared_ptr<Client>& client)
    {
        this->client = client;
    }

    const std::shared_ptr<CommandSet>& getCommandSet() const
    {
        return commandSet;
    }

    void setCommandSet(const std::shared_ptr<CommandSet>& commandSet)
    {
        commandSet->setClient(client);
        this->commandSet = commandSet;
    }

private:
    std::shared_ptr<Client> client;
    std::shared_ptr<CommandSet> commandSet;
};



#endif /* KEYBOARDHANDLER_HPP_ */
