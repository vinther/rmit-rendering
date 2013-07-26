/*
 * Client.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <SDL/SDL_stdinc.h>
#include <memory>
#include <iostream>


class Keyboard;
struct SDL_Surface;
union SDL_Event;

class Client : public std::enable_shared_from_this<Client> {
public:
	Client(int argc, char** argv);
	virtual ~Client();

	void initialize();
	void reshape(Uint32 width, Uint32 height);
	void update(Uint32 ms);
	void event(SDL_Event* event);
	void display(SDL_Surface* surface);

	void cleanup();

    const std::shared_ptr<Keyboard>& getKeyboard() const
    {
        return keyboard;
    }

    void setKeyboard(const std::shared_ptr<Keyboard>& keyboard)
    {
        this->keyboard = keyboard;
    }

private:
	std::shared_ptr<Keyboard> keyboard;
};

#endif /* CLIENT_HPP_ */
