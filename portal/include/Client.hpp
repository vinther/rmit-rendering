/*
 * Client.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <SDL/SDL.h>

class Client {
public:
	Client(int argc, char** argv);
	virtual ~Client();

	void initialize();
	void reshape(Uint32 width, Uint32 height);
	void update(Uint32 ms);
	void event(SDL_Event* event);
	void display(SDL_Surface* surface);

	void cleanup();

private:
};

#endif /* CLIENT_HPP_ */
