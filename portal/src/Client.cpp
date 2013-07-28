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

    keyboardHandler = std::unique_ptr<KeyboardHandler>(new KeyboardHandler());
    keyboardHandler->setClient(shared_from_this());
    keyboardHandler->setCommandSet(std::make_shared<DefaultCommandSet>());

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
}

void Client::reshape(Uint32 width, Uint32 height)
{
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65.0, (GLfloat) width / (GLfloat) height, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
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

        keyboardHandler->event(event->key);

        break;
    case SDL_KEYUP:
        keyboardHandler->event(event->key);

        break;
    }
}

void Client::display(SDL_Surface* surface)
{
//    const char *c;
//    const size_t buffer_size = 128;
//    char string[buffer_size];

    auto sceneObject = scene->assetManager->get<Asset>("models/shuttle.obj");



    glClearColor(1, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    //glutSolidTeapot(1.0f);

    glTranslatef(0.0f, 0.0f, -7.0f);

    if (!sceneObject.expired())
    {
        auto d = sceneObject.lock();


        glEnableClientState(GL_VERTEX_ARRAY);

        for (unsigned int i = 0; i < d->scene->mNumMeshes; ++i)
        {
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

    glMatrixMode(GL_MODELVIEW);

    fflush(stdout);

}

void Client::cleanup()
{
}

