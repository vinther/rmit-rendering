/*
 * Renderer.hpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

class Scene;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    struct Settings
    {
        Uint16 width, height;
        float fov = 65.0;
    } settings;

    struct RenderResults
    {
        Settings settings;
    };

    void initialize();
    void render(const Scene& scene, RenderResults& results);

    // Testing

    std::vector<std::vector<unsigned int>> indices;
};

#endif /* RENDERER_HPP_ */
