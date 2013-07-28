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
#include <vector>


class KeyboardHandler;
class Scene;

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

public:
	std::unique_ptr<KeyboardHandler> keyboardHandler;
	std::unique_ptr<Scene> scene;

	// Testing

	std::vector<std::vector<unsigned int>> indices;
};

#endif /* CLIENT_HPP_ */
