/*
 * Client.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <memory>
#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <SDL2/SDL.h>

class KeyboardHandler;
class MouseHandler;
class Scene;
class Interface;
class CameraController;
class GLRenderer;

struct SDL_Surface;
union SDL_Event;

class Client : public std::enable_shared_from_this<Client>
{
public:
	Client(int argc, char** argv);

	void initialize();
	void reshape(Uint32 width, Uint32 height);
	void update(Uint32 ms);
	void event(SDL_Event* event);
	void display();

	void cleanup();
public:
	std::unique_ptr<KeyboardHandler> keyboardHandler;
	std::unique_ptr<MouseHandler> mouseHandler;
	std::unique_ptr<Scene> scene;
	std::unique_ptr<Interface> interface;
	std::unique_ptr<CameraController> cameraController;
	std::unique_ptr<GLRenderer> renderer;
};

#endif /* CLIENT_HPP_ */
