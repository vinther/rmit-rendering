#ifndef RENDERER_DEBUG_RENDERER_HPP_
#define RENDERER_DEBUG_RENDERER_HPP_

#include <vector>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include "physics/Types.hpp"

#include "scene/scene_graph.hpp"

namespace renderer
{

class debug_renderer
{
public:
    debug_renderer();
    virtual ~debug_renderer();

    struct settings_t
    {
        settings_t()
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

} /* namespace renderer */

#endif /* RENDERER_DEBUG_RENDERER_HPP_ */
