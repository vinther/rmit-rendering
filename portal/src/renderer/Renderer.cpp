/*
 * Renderer.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "renderer/Renderer.hpp"

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

Renderer::Renderer()
{
    // TODO Auto-generated constructor stub

}

Renderer::~Renderer()
{
    // TODO Auto-generated destructor stub
}

void Renderer::render(const Scene& scene, RenderResults& results)
{
    results.settings = settings;

    const auto& camera = *(scene.camera);

    glMatrixMode(GL_MODELVIEW);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto shader = *(scene.assetManager->get<Shader>("shaders/helloWorld"));

    glUseProgram(shader.program);

    shader.renderInfo.model = glGetUniformLocation(shader.program, "model");
    shader.renderInfo.view = glGetUniformLocation(shader.program, "view");
    shader.renderInfo.projection = glGetUniformLocation(shader.program, "projection");

    const auto viewMatrix = glm::mat4_cast(-camera.rotation) * glm::translate(glm::mat4(1.0f), -camera.position);
    const auto projMatrix = glm::perspective(settings.fov, settings.width / (float) settings.height, settings.nearClip, settings.farClip);

    glUniformMatrix4fv(shader.renderInfo.view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader.renderInfo.projection, 1, GL_FALSE, glm::value_ptr(projMatrix));

    renderNode(*(scene.root), camera, shader, glm::mat4(1.0f));

    glUseProgram(0);
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

void Renderer::bufferObject(Model& model)
{
    auto& renderInfo = model.renderInfo;

    if (renderInfo.state &
            (Model::RenderInfo::State::DIRTY | Model::RenderInfo::State::PRISTINE))
    {
        if (renderInfo.vbo.size() > 0)
        {
            glDeleteBuffers(renderInfo.vbo.size(), renderInfo.vbo.data());
            glDeleteBuffers(renderInfo.vao.size(), renderInfo.vao.data());
            glDeleteBuffers(renderInfo.ibo.size(), renderInfo.ibo.data());
            glDeleteBuffers(renderInfo.normals.size(), renderInfo.normals.data());
            glDeleteBuffers(renderInfo.tangents.size(), renderInfo.tangents.data());
            glDeleteBuffers(renderInfo.texCoords.size(), renderInfo.texCoords.data());
        }

        //SDL_Log("No rendering information saved for \"%s\", now generating buffers etc.", model.name.c_str());

        if (model.scene == nullptr)
            throw std::runtime_error("Trying to render scene with NULL scene");

        renderInfo.vbo.resize(model.scene->mNumMeshes);
        renderInfo.vao.resize(model.scene->mNumMeshes);
        renderInfo.ibo.resize(model.scene->mNumMeshes);
        renderInfo.normals.resize(model.scene->mNumMeshes);
        renderInfo.tangents.resize(model.scene->mNumMeshes);
        renderInfo.texCoords.resize(model.scene->mNumMeshes);
        renderInfo.numFaces.resize(model.scene->mNumMeshes);

        glGenVertexArrays(model.scene->mNumMeshes, renderInfo.vao.data());
        glGenBuffers(model.scene->mNumMeshes, renderInfo.vbo.data());
        glGenBuffers(model.scene->mNumMeshes, renderInfo.ibo.data());
        glGenBuffers(model.scene->mNumMeshes, renderInfo.normals.data());
        glGenBuffers(model.scene->mNumMeshes, renderInfo.tangents.data());
        glGenBuffers(model.scene->mNumMeshes, renderInfo.texCoords.data());


        std::vector<GLuint> indices;

        for (unsigned int i = 0; i < model.scene->mNumMeshes; ++i)
        {
            const auto& mesh = *(model.scene->mMeshes[i]);

            indices.resize(mesh.mNumFaces * 3);
            for (unsigned int j = 0; j < mesh.mNumFaces; ++j)
            {
                std::copy(mesh.mFaces[j].mIndices, mesh.mFaces[j].mIndices + 3, indices.data() + j * 3);
            }

            renderInfo.numFaces[i] = mesh.mNumFaces;

            glBindVertexArray(renderInfo.vao[i]);

            glBindBuffer(GL_ARRAY_BUFFER, renderInfo.vbo[i]);
            glBufferData(GL_ARRAY_BUFFER, mesh.mNumVertices * sizeof(aiVector3D), mesh.mVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, renderInfo.normals[i]);
            glBufferData(GL_ARRAY_BUFFER, mesh.mNumVertices * sizeof(aiVector3D), mesh.mNormals, GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, renderInfo.tangents[i]);
            glBufferData(GL_ARRAY_BUFFER, mesh.mNumVertices * sizeof(aiVector3D), mesh.mTangents, GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, renderInfo.texCoords[i]);
            glBufferData(GL_ARRAY_BUFFER, mesh.mNumVertices * sizeof(aiVector3D), mesh.mTextureCoords[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderInfo.ibo[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.mNumFaces * 3 * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
        }

        renderInfo.state = Model::RenderInfo::State::READY;

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

static glm::vec3 cols[10] = {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.5f, 1.0f),
        glm::vec3(0.0f, 0.5f, 0.0f),
        glm::vec3(0.5f, 0.0f, 0.0f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 0.0f, 0.5f),
};

void Renderer::renderNode(SceneNode& node, const Camera& camera, const Shader& activeShader, glm::mat4 modelMatrix)
{
    glPushMatrix();

    auto model = modelMatrix * node.transformation;

    glUniformMatrix4fv(activeShader.renderInfo.model, 1, GL_FALSE, glm::value_ptr(model));

    if (!node.asset.expired())
    {
        auto& model = *std::static_pointer_cast<Model>(node.asset.lock());

        bufferObject(model);

        glEnableClientState(GL_VERTEX_ARRAY);

        renderObject(model);

        glDisableClientState(GL_VERTEX_ARRAY);
    }

    for (auto& childNodePtr: node.children)
    {
        renderNode(*(childNodePtr), camera, activeShader, model);
    }

    glPopMatrix();
}

void Renderer::renderObject(const Model& model)
{
    auto& renderInfo = model.renderInfo;

    for (unsigned int i = 0; i < renderInfo.vbo.size(); ++i)
    {
        glBindVertexArray(renderInfo.vao[i]);

        glColor3fv(glm::value_ptr(cols[i % 10]));
        glDrawElements(GL_TRIANGLES, renderInfo.numFaces[i] * 3, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
}
