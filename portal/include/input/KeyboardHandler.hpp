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

class client;
class CommandSet;

namespace input
{

class keyboard_handler
{
public:
    typedef std::pair<Uint16, Uint16> KeyModPair;

    keyboard_handler(std::shared_ptr<client> client);
    ~keyboard_handler();

    void handle_event(const SDL_KeyboardEvent& event) const;

    void inject_key_down(SDL_Keycode key, Uint16 mod) const;
    void inject_key_up(SDL_Keycode key, Uint16 mod) const;
private:
    std::shared_ptr<client> client;

    struct KeyHasher
    {
        size_t operator()(const KeyModPair& keyPair) const
        {
            return hasher(((Uint32) keyPair.first) << 16 & keyPair.second);
        }

        std::hash<Uint32> hasher;
    };

    std::unordered_map<KeyModPair, std::function<void(client&)>, KeyHasher> keydownMap;
    std::unordered_map<KeyModPair, std::function<void(client&)>, KeyHasher> keyupMap;
};

}

#endif /* KEYBOARDHANDLER_HPP_ */
