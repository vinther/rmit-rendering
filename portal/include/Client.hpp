/*
 * Client.hpp
 *
 *  Created on: 25/07/2013
 *      Author: svp
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <SDL/SDL.h>

class Client
{
public:
	Client(int argc, char** argv);

	void initialize();
	void reshape(Uint32 width, Uint32 height);
	void display(SDL_Surface* surface);
	void update(Uint32 ms);
	void cleanup();
	void event(SDL_Event* event);
};


#endif /* CLIENT_HPP_ */
