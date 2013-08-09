/*
 * KeyboardHandler.cpp
 *
 *  Created on: 26/07/2013
 *      Author: svp
 */

#include "input/MouseHandler.hpp"

#include "client/Client.hpp"
#include "client/CameraController.hpp"

#include "scene/Scene.hpp"
#include "scene/Camera.hpp"

#include "physics/Octree.hpp"

#include "renderer/DebugRenderer.hpp"

input::MouseHandler::MouseHandler(std::shared_ptr<Client> client)
    : client(client)
{
}

void input::MouseHandler::event(const SDL_MouseButtonEvent& event)
{
    switch (event.type)
    {
    case SDL_MOUSEBUTTONDOWN:
        mouseDown(event.button, event.x, event.y);
        break;
    case SDL_MOUSEBUTTONUP:
        mouseUp(event.button, event.x, event.y);
        break;
    }
}

void input::MouseHandler::event(const SDL_MouseMotionEvent& event)
{
    mouseMove(event.x, event.y, event.xrel, event.yrel);
}

void input::MouseHandler::mouseDown(Uint8 button, Uint16 x, Uint16 y)
{
    client->cameraController->mouseDown(button, x, y);

    physics::Octree::IntersectionDetails intersectionResult;
    client->scene->intersectionTree->trace(
            physics::Ray{
                client->scene->camera->position,
                client->scene->camera->forward()},
            intersectionResult
    );

    if (intersectionResult.intersection)
    {
        client->debugRenderer->points.push_back(intersectionResult.position);
    }
}

void input::MouseHandler::mouseUp(Uint8 button, Uint16 x, Uint16 y)
{
    client->cameraController->mouseUp(button, x, y);
}

void input::MouseHandler::mouseMove(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel)
{
    client->cameraController->mouseMove(x, y, xrel, yrel);
}


