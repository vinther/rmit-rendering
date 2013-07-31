
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
    : data()
    , client(client)
{
}

void Interface::display()
{
    std::stringstream sstream;

    glColor3f(1.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    glOrtho(0, 512, 0, 512, 0, 1000);

    glMatrixMode(GL_MODELVIEW);

    sstream << "FPS: " << data.fps;
    writeText(sstream.str(), 20.0f, 16.0f);

    sstream.str("");
    sstream << "Cam: <" << data.camPos.x << ", " << data.camPos.y << ", " << data.camPos.z << ">";
    writeText(sstream.str(), 20.0f, 32.0f);

    sstream.str("");
    sstream << "RenderTime: " << data.renderTime;
    writeText(sstream.str(), 20.0f, 48.0f);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void Interface::writeText(const std::string& str, float x, float y)
{
    const char* c_str = str.c_str();

    glRasterPos2f(x, y);
    for (const char* c = c_str; *c != '\0'; ++c) {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}


