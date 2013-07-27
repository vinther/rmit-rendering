/*
 * Keyboard.hpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include <SDL/SDL.h>

#include "Client.hpp"
#include "CommandSet.hpp"

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



#endif /* KEYBOARD_HPP_ */
