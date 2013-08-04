/*
 * CameraController.hpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#ifndef CAMERACONTROLLER_HPP_
#define CAMERACONTROLLER_HPP_

#include <memory>

#include <SDL2/SDL.h>

class Client;

class CameraController
{
public:
    CameraController(std::shared_ptr<Client> client);
    virtual ~CameraController();

    void keyDown(SDL_Keycode key, Uint16 mod);
    void keyUp(SDL_Keycode key, Uint16 mod);
    void mouseDown(Uint8 button, Uint16 x, Uint16 y);
    void mouseUp(Uint8 button, Uint16 x, Uint16 y);
    void mouseMove(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel);

    void update(Uint32 ms);

    struct State
    {
        State();

        bool moveForward;
        bool moveBackward;
        bool moveLeft;
        bool moveRight;
        bool moveUp;
        bool moveDown;

        float speedFactor;
    } state;
private:
    std::shared_ptr<Client> client;
};

#endif /* CAMERACONTROLLER_HPP_ */
