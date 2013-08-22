/*
 * Keyboard.hpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#ifndef KEYBOARDHANDLER_HPP_
#define KEYBOARDHANDLER_HPP_

#include <memory>
#include <unordered_map>

#include <SDL2/SDL.h>

class Client;
class CommandSet;

namespace input
{

class KeyboardHandler
{
public:
    typedef std::pair<Uint16, Uint16> KeyModPair;

    KeyboardHandler(std::shared_ptr<Client> client);
    ~KeyboardHandler();

    void event(const SDL_KeyboardEvent& event) const;

    void keydown(SDL_Keycode key, Uint16 mod) const;
    void keyup(SDL_Keycode key, Uint16 mod) const;
private:
    std::shared_ptr<Client> client;

    struct KeyHasher
    {
        size_t operator()(const KeyModPair& keyPair) const
        {
            return hasher(((Uint32) keyPair.first) << 16 & keyPair.second);
        }

        static std::hash<Uint32> hasher;
    };

    std::unordered_map<KeyModPair, std::function<void(Client&)>, KeyHasher> keydownMap;
    std::unordered_map<KeyModPair, std::function<void(Client&)>, KeyHasher> keyupMap;
};

}

#endif /* KEYBOARDHANDLER_HPP_ */
