#ifndef DEMO_CLIENT_HPP_
#define DEMO_CLIENT_HPP_

#include <memory>
#include <iostream>
#include <vector>
#include <chrono>

#include <GL/glut.h>
#include <SDL2/SDL.h>

#include "renderer/renderer.hpp"
#include "renderer/debug_renderer.hpp"
#include "renderer/interface_renderer.hpp"

namespace input
{
class keyboard_handler;
class mouse_handler;
}

namespace scene
{
class scene_graph;
}

namespace assets
{
class data_store;
}

namespace threading
{
class thread_pool;
}

class camera_controller;


struct SDL_Surface;
union SDL_Event;

class client : public std::enable_shared_from_this<client>
{
public:
	client(int argc, char** argv);

	void initialize(SDL_Window* window, SDL_GLContext context);
	void reshape(Uint32 width, Uint32 height);

	void prepareFrame(SDL_Window* window, SDL_GLContext context);
	void finalizeFrame(SDL_Window* window, SDL_GLContext context);

	void event(SDL_Event* event);
	void display();

	void cleanup();

	std::unique_ptr<input::keyboard_handler> keyboardHandler;
	std::unique_ptr<input::mouse_handler> mouseHandler;
	std::unique_ptr<scene::scene_graph> scene;

	std::unique_ptr<camera_controller> cameraController;
    std::unique_ptr<assets::data_store> assetManager;
	std::unique_ptr<renderer::renderer> renderer;
	std::unique_ptr<renderer::interface_renderer> interface;
	std::unique_ptr<renderer::debug_renderer> debugRenderer;

	std::unique_ptr<threading::thread_pool> threadPool;

    std::function<void(void *userdata, int category, SDL_LogPriority priority, const char *message)> logFunction;
private:
	std::chrono::high_resolution_clock::time_point timeFrameBegin;
	std::chrono::high_resolution_clock::time_point timeFrameEnd;

	unsigned long long frameCount;

	std::chrono::high_resolution_clock::time_point timeFrameFPS;
	unsigned int frameCountFPS;

	SDL_LogOutputFunction defaultLogOutputFunction;
};

#endif /* DEMO_CLIENT_HPP_ */
