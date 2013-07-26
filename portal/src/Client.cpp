/*
 * Client.cpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#include "Client.hpp"

#include <iostream>

#include <SDL/SDL.h>
#include <GL/glut.h>

#include "Keyboard.hpp"

Client::Client(int argc, char** argv)
{
    // TODO Auto-generated constructor stub

}

Client::~Client()
{
    // TODO Auto-generated destructor stub
}

void Client::initialize()
{
    //int argc = 0;
    //char const * argv[] = {"prog.exe","-k"};

    //glutInit(&argc, argv);

    setKeyboard(std::make_shared<Keyboard>());

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void Client::reshape(Uint32 width, Uint32 height)
{
}

void Client::update(Uint32 ms)
{
}

void Client::event(SDL_Event* event)
{
    if (0 == event)
        return;

    switch (event->type)
    {
    case SDL_KEYDOWN:
        if (SDLK_ESCAPE == event->key.keysym.sym)
        {
            SDL_Event quitEvent;
            quitEvent.type = SDL_QUIT;

            SDL_PushEvent(&quitEvent);

        }

        getKeyboard()->event(event->key);

        break;
    case SDL_KEYUP:
        getKeyboard()->event(event->key);

        break;
    }
}

void Client::display(SDL_Surface* surface)
{
}

void Client::cleanup()
{
}

