/*
 * DebugRenderer.hpp
 *
 *  Created on: 08/08/2013
 *      Author: svp
 */

#ifndef DEBUGRENDERER_HPP_
#define DEBUGRENDERER_HPP_

#include <vector>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "physics/Types.hpp"

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
            , drawBVH(false), drawCrosshair(false), drawRays(false)
        {}

        Uint16 width, height;
        bool drawBVH, drawCrosshair, drawRays;
    } settings;

    void initialize();
    void render(const scene::Scene& scene);

    std::vector<physics::Line> lines;
};

} /* namespace threading */
#endif /* DEBUGRENDERER_HPP_ */
