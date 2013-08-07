/*
 * Interface.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef INTERFACE_HPP_
#define INTERFACE_HPP_

#include <string>
#include <memory>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "scene/Camera.hpp"

class Client;

namespace assets
{
class Font;
}

typedef unsigned int GLuint;

class Interface
{
public:
    Interface(std::shared_ptr<Client> client);
    ~Interface();

    bool initialize();

	struct Data
	{
	    Uint16 fps;
	    Uint16 renderTime;

	    scene::Camera::State cameraState;
	} data;

	struct Settings
	{
	    Uint16 width, height;
	    std::string font;
	    Uint16 ptSize;
	} settings;

    void display(SDL_Renderer* sdlRenderer);
private:
    std::shared_ptr<assets::Font> font;

    std::shared_ptr<Client> client;
    bool initialized;

    SDL_Surface* surface;
    GLuint texture;

    void writeText(const std::string& str, float x, float y);
};

#endif /* INTERFACE_HPP_ */
