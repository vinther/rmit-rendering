/*
 * DebugRenderer.hpp
 *
 *  Created on: 08/08/2013
 *      Author: svp
 */

#ifndef RENDERER_DEBUG_RENDERER_HPP_
#define RENDERER_DEBUG_RENDERER_HPP_

#include <vector>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "physics/Types.hpp"

namespace scene
{
class scene_graph;
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
    void render(const scene::scene_graph& scene);

    std::vector<physics::Line> lines;
};

} /* namespace threading */
#endif /* RENDERER_DEBUG_RENDERER_HPP_ */
