/*
 * Renderer.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "renderer/GLRenderer.hpp"

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

#include "renderer/GLResourceManager.hpp"
#include "renderer/GLBufferedModel.hpp"
#include "renderer/GLBufferedTexture.hpp"
#include "renderer/GLBufferedMaterial.hpp"
#include "renderer/GLBufferedShader.hpp"

#include "Utilities.hpp"

GLRenderer::GLRenderer()
    : resourceManager(std::make_unique<GLResourceManager>())
{
    // TODO Auto-generated constructor stub

}

GLRenderer::~GLRenderer()
{
    // TODO Auto-generated destructor stub
}

GLuint buffer = 0, bufferLight = 0;

float lightTime = 0.0f;
float lightPos = 0.0f;

struct LightSourceParameters
{
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  glm::vec4 position;
} lights[8];

void GLRenderer::render(const Scene& scene, RenderResults& results)
{
    using namespace std::chrono;
    const auto timeBegin = high_resolution_clock::now();

    results.settings = settings;

    const auto& camera = *(scene.camera);

    glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 239.0f / 255.0f, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    const auto shader = resourceManager->getByHash<GLBufferedShader>(shaderHash);

    glUseProgram(shader.program);

    if (0 == buffer)
    {
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);

        GLuint bindingPoint = 1, blockIndex;
//        blockIndexLights = glGetUniformBlockIndex(shader.program, "LightSourceParametersLoc");
        blockIndex = glGetUniformBlockIndex(shader.program, "MaterialParametersLoc");

        glUniformBlockBinding(shader.program, blockIndex, bindingPoint);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);
    }

    modelMatrixLoc = glGetUniformLocation(shader.program, "model");
    viewMatrixLoc = glGetUniformLocation(shader.program, "view");
    projectionMatrix = glGetUniformLocation(shader.program, "projection");

    const auto viewMatrix = glm::mat4_cast(-camera.rotation) * glm::translate(glm::mat4(1.0f), -camera.position);
    const auto projMatrix = glm::perspective(settings.fov, settings.width / (float) settings.height, settings.nearClip, settings.farClip);

    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix));

    glEnableClientState(GL_VERTEX_ARRAY);

    if (scene.root->model)
    {
        renderNode(*(scene.root), camera, glm::mat4(1.0f));
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glUseProgram(0);

    const auto timeEnd = high_resolution_clock::now();

    results.renderTime = duration_cast<microseconds>(timeEnd-timeBegin);
}

void GLRenderer::initialize()
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

void GLRenderer::renderNode(SceneNode& node, const Camera& camera, glm::mat4 modelMatrix)
{
    glPushMatrix();

    auto model = modelMatrix * node.transformation;

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(model));

    renderModel(resourceManager->getByAsset<GLBufferedModel>(node.model));

    for (auto& childNodePtr: node.children)
    {
        renderNode(*(childNodePtr), camera, model);
    }

    glPopMatrix();
}

void GLRenderer::renderModel(const GLBufferedModel& model)
{
    if (!(model.state & GLBufferedModel::State::READY))
        return;

    std::string currentMaterialName = "";

    lightTime += 1.0f;
    lights[0].ambient = glm::normalize(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f));

    glBindBufferBase(GL_UNIFORM_BUFFER, 2, buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightSourceParameters) * 8, &lights, GL_DYNAMIC_DRAW);

    for (const auto& mesh: model.meshes)
    {
        const GLBufferedMaterial& material = resourceManager->getByHash<GLBufferedMaterial>(mesh.materialHash);
        material.activate(*resourceManager);

//
//        //SDL_Log("SPAM: Rendering mesh %d. VAO: %d., VBO: %d, faces: %d", 0, mesh.vao, mesh.buffers[0], mesh.numFaces * 3);
//

        glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(GLBufferedMaterial::MaterialInfo), &material.materialInfo, GL_DYNAMIC_DRAW);

            //SDL_Log("Switched material to \"%s\": %f, %f, %f", currentMaterialName.c_str(),
            //        mesh.material->materialInfo.diffuse.x, mesh.material->materialInfo.diffuse.y, mesh.material->materialInfo.diffuse.z);


        glBindVertexArray(mesh.vao);

        glDrawElements(GL_TRIANGLES, mesh.numFaces * 3, GL_UNSIGNED_INT, 0);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}
