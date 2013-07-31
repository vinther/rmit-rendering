/*
 * GLResourceManager.cpp
 *
 *  Created on: 01/08/2013
 *      Author: svp
 */

#include "renderer/GLResourceManager.hpp"

#include <stdexcept>

#include <GL/glew.h>
#include <GL/gl.h>
#include <assimp/scene.h>

#include "scene/Scene.hpp"

#include "assets/Model.hpp"
#include "assets/Shader.hpp"
#include "assets/Texture.hpp"

GLResourceManager::GLResourceManager()
{
    // TODO Auto-generated constructor stub

}

GLResourceManager::~GLResourceManager()
{
    // TODO Auto-generated destructor stub
}

template<>
Model& GLResourceManager::bufferObject<Model>(Model& model)
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

    return model;
}

