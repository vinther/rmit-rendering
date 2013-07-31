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
#include <glm/glm.hpp>

class Scene;
class SceneNode;
class Camera;

class Model;
class Shader;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    struct Settings
    {
        Uint16 width, height;
        float fov = 65.0, nearClip = 1.0f, farClip = 2000.0f;
    } settings;

    struct RenderResults
    {
        Settings settings;
    };

    void initialize();
    void render(const Scene& scene, RenderResults& results);

private:
    void bufferObject(Model& model);
    void renderNode(SceneNode& node, const Camera& camera, const Shader& activeShader, glm::mat4 modelMatrix);
    void renderObject(const Model& model);
};

#endif /* RENDERER_HPP_ */
