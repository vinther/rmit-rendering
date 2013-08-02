/*
 * Renderer.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "renderer/Renderer.hpp"

#include <stdexcept>
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
#include "renderer/Material.hpp"

Renderer::Renderer()
{
    // TODO Auto-generated constructor stub

}

Renderer::~Renderer()
{
    // TODO Auto-generated destructor stub
}

GLuint buffer = 0;

void Renderer::render(const Scene& scene, RenderResults& results)
{
    using namespace std::chrono;
    const auto timeBegin = high_resolution_clock::now();

    results.settings = settings;

    const auto& camera = *(scene.camera);

    glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 239.0f / 255.0f, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto shader = *(scene.assetManager->get<Shader>("shaders/helloWorld"));

    glUseProgram(shader.renderInfo.program);

    if (0 == buffer)
    {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);

        GLuint bindingPoint = 1, blockIndex;
        blockIndex = glGetUniformBlockIndex(shader.renderInfo.program, "UBOData");
        glUniformBlockBinding(shader.renderInfo.program, blockIndex, bindingPoint);

        float myFloats[8] = {1.0, 0.0, 0.0, 1.0,   0.4, 0.0, 0.0, 1.0};

        glBufferData(GL_UNIFORM_BUFFER, sizeof(myFloats), myFloats, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
    }

    shader.renderInfo.model = glGetUniformLocation(shader.renderInfo.program, "model");
    shader.renderInfo.view = glGetUniformLocation(shader.renderInfo.program, "view");
    shader.renderInfo.projection = glGetUniformLocation(shader.renderInfo.program, "projection");

    const auto viewMatrix = glm::mat4_cast(-camera.rotation) * glm::translate(glm::mat4(1.0f), -camera.position);
    const auto projMatrix = glm::perspective(settings.fov, settings.width / (float) settings.height, settings.nearClip, settings.farClip);

    glUniformMatrix4fv(shader.renderInfo.view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader.renderInfo.projection, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glEnableClientState(GL_VERTEX_ARRAY);

    renderNode(*(scene.root), camera, shader, glm::mat4(1.0f), *(scene.assetManager));

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
}

void Renderer::renderNode(SceneNode& node, const Camera& camera, const Shader& activeShader, glm::mat4 modelMatrix, AssetManager& assetManager)
{
    glPushMatrix();

    auto model = modelMatrix * node.transformation;

    glUniformMatrix4fv(activeShader.renderInfo.model, 1, GL_FALSE, glm::value_ptr(model));

    if (!node.model.expired())
    {
        auto object = node.model.lock();

        renderModel(resourceManager->bufferObject(*object, assetManager));
    }

    for (auto& childNodePtr: node.children)
    {
        renderNode(*(childNodePtr), camera, activeShader, model, assetManager);
    }

    glPopMatrix();
}

void Renderer::renderModel(const Model& model)
{
    if (!(model.renderInfo.state & Model::RenderInfo::State::READY))
        return;

    std::string currentMaterialName = "";

    for (const auto& mesh: model.renderInfo.meshes)
    {
        mesh.material->activate();

        //SDL_Log("SPAM: Rendering mesh %d. VAO: %d., VBO: %d, faces: %d", 0, mesh.vao, mesh.buffers[0], mesh.numFaces * 3);

        if (mesh.material->name != currentMaterialName)
        {
            glBufferData(GL_UNIFORM_BUFFER, 4 * 4 * 4, &mesh.material->materialInfo, GL_DYNAMIC_DRAW);
            currentMaterialName = mesh.material->name;

            //SDL_Log("Switched material to \"%s\"", currentMaterialName.c_str());
        }

        glBindVertexArray(mesh.vao);

        glDrawElements(GL_TRIANGLES, mesh.numFaces * 3, GL_UNSIGNED_INT, 0);
    }

    //sssssssssssssssssssssssthrow std::runtime_error("Enough");

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}
