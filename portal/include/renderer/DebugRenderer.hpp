/*
 * DebugRenderer.hpp
 *
 *  Created on: 08/08/2013
 *      Author: svp
 */

#ifndef DEBUGRENDERER_HPP_
#define DEBUGRENDERER_HPP_

#include <SDL2/SDL.h>

namespace scene
{
class Scene;
}

namespace renderer
{

class DebugRenderer
{
public:
    DebugRenderer();
    virtual ~DebugRenderer();

    struct Settings
    {
        Settings()
            : width(640), height(480)
            , fov(65.0f), nearClip(1.0f), farClip(10000.0f)
            , enabled(false)
            , maxTreeDepth(4)
        {}

        Uint16 width, height;
        float fov, nearClip, farClip;
        bool enabled;
        unsigned int maxTreeDepth;
    } settings;

    void initialize();
    void render(const scene::Scene& scene);
};

} /* namespace threading */
#endif /* DEBUGRENDERER_HPP_ */
