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
#include <chrono>

#include <GL/glut.h>
#include <SDL2/SDL.h>

namespace input
{
class KeyboardHandler;
class MouseHandler;
}

namespace scene
{
class Scene;
}

namespace assets
{
class AssetManager;
}

namespace threading
{
class ThreadPool;
}

namespace renderer
{
class Renderer;
class DebugRenderer;
}

class Interface;
class CameraController;


struct SDL_Surface;
union SDL_Event;

class Client : public std::enable_shared_from_this<Client>
{
public:
	Client(int argc, char** argv);

	void initialize(SDL_Window* window, SDL_Renderer* renderer);
	void reshape(Uint32 width, Uint32 height);

	void prepareFrame();
	void finalizeFrame();

	void event(SDL_Event* event);
	void display();

	void cleanup();

	std::unique_ptr<input::KeyboardHandler> keyboardHandler;
	std::unique_ptr<input::MouseHandler> mouseHandler;
	std::unique_ptr<scene::Scene> scene;
	std::unique_ptr<Interface> interface;
	std::unique_ptr<CameraController> cameraController;

    std::unique_ptr<assets::AssetManager> assetManager;
	std::unique_ptr<renderer::Renderer> renderer;
	std::unique_ptr<renderer::DebugRenderer> debugRenderer;

	std::unique_ptr<threading::ThreadPool> threadPool;

private:
	SDL_Window* sdlWindow;
	SDL_Renderer* sdlRenderer;

	std::chrono::high_resolution_clock::time_point timeFrameBegin;
	std::chrono::high_resolution_clock::time_point timeFrameEnd;

	unsigned long long frameCount;

	std::chrono::high_resolution_clock::time_point timeFrameFPS;
	unsigned int frameCountFPS;
};

#endif /* CLIENT_HPP_ */
