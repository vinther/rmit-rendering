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

#include "scene/Scene.hpp"

class DefaultCommandSet: public CommandSet
{
public:
    void keydown(SDLKey key, SDLMod mod)
    {
        switch (key)
        {
        case 'a':
            client->scene->camera->setPosition(glm::vec3(1.0f, 1.0f, 1.0f));
            break;
        default:
            break;
        }
    }

    void keyup(SDLKey key, SDLMod mod)
    {

    }

};

#endif /* DEFAULTCOMMANDSET_HPP_ */
