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

#include "input/KeyboardHandler.hpp"
#include "input/MouseHandler.hpp"
#include "input/DefaultCommandSet.hpp"

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
    char fakeParam[] = "";
    char *fakeargv[] = {fakeParam};
    int argc = 0;

    glutInit( &argc, fakeargv );

    auto sharedCommandSet = std::make_shared<DefaultCommandSet>();
    sharedCommandSet->setClient(shared_from_this());

    keyboardHandler = std::unique_ptr<KeyboardHandler>(new KeyboardHandler());
    keyboardHandler->setClient(shared_from_this());
    keyboardHandler->setCommandSet(sharedCommandSet);

    mouseHandler = std::unique_ptr<MouseHandler>(new MouseHandler());
    mouseHandler->client = shared_from_this();
    mouseHandler->commandSet = sharedCommandSet;

    scene = std::unique_ptr<Scene>(new Scene());
//    auto sceneObject = scene->assetManager->get<Asset>("models/shuttle.obj");
//
//    if (!sceneObject.expired())
//    {
//        auto d = sceneObject.lock();
//        std::cout << d->scene->mNumMeshes << std::endl;
//    }
//
//    sceneObject = scene->assetManager->get<Asset>("models/shuttle.obj");
//    if (!sceneObject.expired())
//    {
//        auto d = sceneObject.lock();
//        std::cout << d->scene->mNumMeshes << std::endl;
//    }

    auto sceneObject = scene->assetManager->get<Asset>("models/shuttle.obj");

    if (!sceneObject.expired())
    {
        auto d = sceneObject.lock();

        indices.resize(d->scene->mNumMeshes);
        for (unsigned int i = 0; i < d->scene->mNumMeshes; ++i)
        {
            for (unsigned int j = 0; j < d->scene->mMeshes[i]->mNumFaces; ++j)
            {
                std::copy(d->scene->mMeshes[i]->mFaces[j].mIndices, d->scene->mMeshes[i]->mFaces[j].mIndices + 3, std::back_inserter(indices[i]));
            }
        }

    }

    glEnable(GL_DEPTH_TEST);    // Enable the depth buffer
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Ask for nicest perspective correction

    glEnable(GL_CULL_FACE);     // Cull back facing polygons
    glCullFace(GL_BACK);
}

void Client::reshape(Uint32 width, Uint32 height)
{
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) width / (GLfloat) height, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>


float rot = 0.0f;
glm::quat r;

void Client::update(Uint32 ms)
{
    r = glm::angleAxis(rot, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    r = glm::normalize(r);
    rot += (0.01f) * ms;

    if (rot > 360)
        rot = 0.0f;
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

        keyboardHandler->event(event->key);

        break;
    case SDL_KEYUP:
        keyboardHandler->event(event->key);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mouseHandler->event(event->button);
        break;
    case SDL_MOUSEBUTTONUP:
        mouseHandler->event(event->button);
        break;
    case SDL_MOUSEMOTION:
        mouseHandler->event(event->motion);
        break;
    default:
        break;
    }
}


void Client::display(SDL_Surface* surface)
{
    auto sceneObject = scene->assetManager->get<Asset>("models/shuttle.obj");


    glMatrixMode(GL_MODELVIEW);

    glClearColor(1, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -20.0f);
    glMultMatrixf(glm::value_ptr(glm::mat4_cast(r)));

    static glm::vec3 cols[10] = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 0.5f, 1.0f),
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.5f, 0.0f, 0.0f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(1.0f, 0.0f, 0.5f),
    };

    if (!sceneObject.expired())
    {
        auto d = sceneObject.lock();

        glEnableClientState(GL_VERTEX_ARRAY);

        for (unsigned int i = 0; i < d->scene->mNumMeshes; ++i)
        {
            glColor3fv(glm::value_ptr(cols[i]));
            glVertexPointer(3, GL_FLOAT, 0, d->scene->mMeshes[i]->mVertices);
            glDrawElements(GL_TRIANGLES, indices[i].size(), GL_UNSIGNED_INT, indices[i].data());
        }

        glDisableClientState(GL_VERTEX_ARRAY);
    }


    glPopMatrix();

//    /* Drawing text */
//    glMatrixMode(GL_PROJECTION);
//    glPushMatrix();
//
//    glLoadIdentity();
//    glOrtho(0, 512, 0, 512, 0, 1000);
//
//    glMatrixMode(GL_MODELVIEW);
//
//    snprintf(string, buffer_size, "%5d FPS, %d yaw\r", fps, yaw);
//
//    glRasterPos2f(text_x, text_y);
//    for (c = string; *c != '\0'; c++) {
//      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
//    }
//
//    glMatrixMode(GL_PROJECTION);
//    glPopMatrix();
//    /* Back to normal drawing */
}

void Client::cleanup()
{
}

