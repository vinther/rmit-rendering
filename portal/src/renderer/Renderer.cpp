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

#include "scene/Scene.hpp"
#include "scene/SceneNode.hpp"
#include "scene/Camera.hpp"

#include "renderer/ResourceManager.hpp"
#include "renderer/BufferedModel.hpp"
#include "renderer/BufferedTexture.hpp"
#include "renderer/BufferedMaterial.hpp"
#include "renderer/BufferedShader.hpp"

#include "Utilities.hpp"

renderer::Renderer::Renderer()
    : resourceManager(std::make_unique<ResourceManager>())
{
    // TODO Auto-generated constructor stub

}

renderer::Renderer::~Renderer()
{
    // TODO Auto-generated destructor stub
}

void renderer::Renderer::initialize()
{
    glewInit();

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);
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

float test = 0.0f;

void renderer::Renderer::render(const scene::Scene& scene, RenderResults& results)
{
    using namespace std::chrono;
    const auto timeBegin = high_resolution_clock::now();

    results.settings = settings;

    test += 0.01f;
    const auto& camera = *(scene.camera);

    glClearColor(125.0f / 255.0f, 190.0f / 255.0f, 239.0f / 255.0f, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    const auto shader = resourceManager->getByHash<BufferedShader>(shaderHash);

    glUseProgram(shader.program);

    glUniform1i(shader.getUniformLocation("ambientTexSampler"), 0);
    glUniform1i(shader.getUniformLocation("diffuseTexSampler"), 1);
    glUniform1i(shader.getUniformLocation("specularTexSampler"), 2);
    glUniform1i(shader.getUniformLocation("normalTexSampler"), 3);
    glUniform1i(shader.getUniformLocation("bumpTexSampler"), 4);

    glUniform1f(shader.getUniformLocation("test"), test);

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


    const glm::mat4 viewProjectionMatrix = camera.getViewProjectionMatrix();

    glUniformMatrix4fv(shader.getUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));

    glEnableClientState(GL_VERTEX_ARRAY);

    RenderState state;

    state.modelMatrix = glm::mat4(1.0f);
    state.locations.modelMatrix = shader.getUniformLocation("model");

    if (scene.root->model)
    {
        renderNode(*(scene.root), state);
    }

    glDisableClientState(GL_VERTEX_ARRAY);

    glUseProgram(0);

    const auto timeEnd = high_resolution_clock::now();

    results.renderTime = duration_cast<microseconds>(timeEnd-timeBegin);
}

void renderer::Renderer::renderNode(scene::SceneNode& node, RenderState state)
{
    glPushMatrix();

    state.modelMatrix = state.modelMatrix * node.transformation;

    glUniformMatrix4fv(state.locations.modelMatrix, 1, GL_FALSE, glm::value_ptr(state.modelMatrix));

    renderModel(resourceManager->getByAsset<BufferedModel>(node.model));

    for (auto& childNodePtr: node.children)
    {
        renderNode(*(childNodePtr), state);
    }

    glPopMatrix();
}

void renderer::Renderer::renderModel(const BufferedModel& model)
{
    if (!(model.state & BufferedModel::State::READY))
        return;

    std::string currentMaterialName = "";

    lightTime += 1.0f;
    lights[0].ambient = glm::normalize(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f));

    glBindBufferBase(GL_UNIFORM_BUFFER, 2, buffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightSourceParameters) * 8, &lights, GL_DYNAMIC_DRAW);

    for (const auto& mesh: model.meshes)
    {
        const BufferedMaterial& material = resourceManager->getByHash<BufferedMaterial>(mesh.materialHash);
        material.activate(*resourceManager);

        glBindBufferBase(GL_UNIFORM_BUFFER, 1, buffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(BufferedMaterial::MaterialInfo), &material.materialInfo, GL_DYNAMIC_DRAW);

        glBindVertexArray(mesh.vao);

        glDrawElements(GL_TRIANGLES, mesh.numFaces * 3, GL_UNSIGNED_INT, 0);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}
