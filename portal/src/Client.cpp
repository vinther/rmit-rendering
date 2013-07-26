/*
 * Client.cpp
 *
<<<<<<< HEAD
 *  Created on: 25/07/2013
 *      Author: svp
=======
 *  Created on: 26/07/2013
 *      Author: s3443408
>>>>>>> fd3f252442028f3bf70ad1cf9a099bb9de1dbbf5
 */

#include "Client.hpp"

<<<<<<< HEAD
Client::Client(int argc, char** argv)
{

}



=======
#include <iostream>

#include <GL/glut.h>

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
    int argc;
    char *argv = "";
    glutInit(&argc, &argv);

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
        else
        {
            std::cout << "Key was pressed!" << std::endl;
        }

        break;
    }
}

void Client::display(SDL_Surface* surface)
{
}

void Client::cleanup()
{
}
>>>>>>> fd3f252442028f3bf70ad1cf9a099bb9de1dbbf5

