
/*
 * Interface.cpp
 *
 *  Created on: 29/07/2013
 *      Author: s3443408
 */


#include "client/Interface.hpp"

#include <sstream>

#include <GL/glut.h>

#include "client/Client.hpp"

Interface::Interface(std::shared_ptr<Client> client)
    : client(client)
{
}

void Interface::display()
{
    /* Drawing text */
    glColor3f(1.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    glOrtho(0, 512, 0, 512, 0, 1000);

    glMatrixMode(GL_MODELVIEW);

    std::stringstream sstream;
    sstream << "FPS: " << data.fps;

    glRasterPos2f(20, 20);
    writeText(sstream.str());

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    /* Back to normal drawing */
}

void Interface::writeText(const std::string& str)
{
    const char* c_str = str.c_str();

    for (const char* c = c_str; *c != '\0'; ++c) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}


