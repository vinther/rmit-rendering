/*
 * DefaultCommandSet.hpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#ifndef DEFAULTCOMMANDSET_HPP_
#define DEFAULTCOMMANDSET_HPP_

#include "input/CommandSet.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "scene/Scene.hpp"

class DefaultCommandSet: public CommandSet
{
public:
    void keydown(SDL_Keycode key, Uint16 mod)
    {
        switch (key)
        {
        case 'w':
            client->scene->camera->position += client->scene->camera->rotation * glm::vec3(0.0f, 0.0f, -1.0f);
            break;
        case 's':
            client->scene->camera->position += client->scene->camera->rotation * glm::vec3(0.0f, 0.0f,  1.0f);
            break;
        case 'd':
            client->scene->camera->position += client->scene->camera->rotation * glm::vec3(1.0f, 0.0f,  0.0f);
            break;
        case 'a':
            client->scene->camera->position += client->scene->camera->rotation * glm::vec3(-1.0f, 0.0f, 0.0f);
            break;
        default:
            break;
        }
    }

    void keyup(SDL_Keycode key, Uint16 mod)
    {

    }

};

#endif /* DEFAULTCOMMANDSET_HPP_ */
