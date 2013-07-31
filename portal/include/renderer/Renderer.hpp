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
#include <chrono>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

class Scene;
class SceneNode;
class Camera;

class Model;
class Shader;

class GLResourceManager;

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    struct Settings
    {
        Uint16 width = 640, height = 480;
        float fov = 65.0, nearClip = 1.0f, farClip = 2000.0f;
    } settings;

    struct RenderResults
    {
        Settings settings = Settings();
        std::chrono::microseconds renderTime = std::chrono::microseconds(0);
    };

    struct RenderState
    {

    };

    void initialize();
    void render(const Scene& scene, RenderResults& results);

    std::unique_ptr<GLResourceManager> resourceManager;
private:
    void renderNode(SceneNode& node, const Camera& camera, const Shader& activeShader, glm::mat4 modelMatrix);
    void renderModel(const Model& model);
};

#endif /* RENDERER_HPP_ */
