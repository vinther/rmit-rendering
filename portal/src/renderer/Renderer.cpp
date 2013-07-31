/*
 * Renderer.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "renderer/Renderer.hpp"

#include <chrono>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>

#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"
#include "scene/Camera.hpp"

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Shader.hpp"

#include "renderer/GLResourceManager.hpp"

#include "Utilities.hpp"

Renderer::Renderer()
    : settings()
    , resourceManager(std::make_unique<GLResourceManager>())
{
    // TODO Auto-generated constructor stub

}

Renderer::~Renderer()
{
    // TODO Auto-generated destructor stub
}

void Renderer::render(const Scene& scene, RenderResults& results)
{
    using namespace std::chrono;
    const auto timeBegin = high_resolution_clock::now();

    results.settings = settings;

    const auto& camera = *(scene.camera);

    glMatrixMode(GL_MODELVIEW);

    glClearColor(125.0f / 255.0f, 235.0f / 255.0f, 239.0f / 255.0f, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto shader = *(scene.assetManager->get<Shader>("shaders/helloWorld"));

    glUseProgram(shader.renderInfo.program);

    shader.renderInfo.model = glGetUniformLocation(shader.renderInfo.program, "model");
    shader.renderInfo.view = glGetUniformLocation(shader.renderInfo.program, "view");
    shader.renderInfo.projection = glGetUniformLocation(shader.renderInfo.program, "projection");

    const auto viewMatrix = glm::mat4_cast(-camera.rotation) * glm::translate(glm::mat4(1.0f), -camera.position);
    const auto projMatrix = glm::perspective(settings.fov, settings.width / (float) settings.height, settings.nearClip, settings.farClip);

    glUniformMatrix4fv(shader.renderInfo.view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader.renderInfo.projection, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glEnableClientState(GL_VERTEX_ARRAY);

    renderNode(*(scene.root), camera, shader, glm::mat4(1.0f));

    glDisableClientState(GL_VERTEX_ARRAY);

    glUseProgram(0);

    const auto timeEnd = high_resolution_clock::now();

    results.renderTime = duration_cast<microseconds>(timeEnd-timeBegin);
}

#include <SDL2/SDL_image.h>

void Renderer::initialize()
{
    glewInit();

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    glCullFace(GL_BACK);

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    auto image = IMG_Load("assets/models/sibenik/kamen.png");

    if (image)
    {
        SDL_Log("Bytes per pixel: %d, w %d, h %d", image->format->BitsPerPixel, image->w, image->h);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No luck with the image");
    }
}

void Renderer::renderNode(SceneNode& node, const Camera& camera, const Shader& activeShader, glm::mat4 modelMatrix)
{
    glPushMatrix();

    auto model = modelMatrix * node.transformation;

    glUniformMatrix4fv(activeShader.renderInfo.model, 1, GL_FALSE, glm::value_ptr(model));

    if (!node.model.expired())
    {
        auto object = node.model.lock();

        renderModel(resourceManager->bufferObject(*object));
    }

    for (auto& childNodePtr: node.children)
    {
        renderNode(*(childNodePtr), camera, activeShader, model);
    }

    glPopMatrix();
}

void Renderer::renderModel(const Model& model)
{
    auto& renderInfo = model.renderInfo;

    for (unsigned int i = 0; i < renderInfo.vbo.size(); ++i)
    {
        glBindVertexArray(renderInfo.vao[i]);

        glDrawElements(GL_TRIANGLES, renderInfo.numFaces[i] * 3, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}
