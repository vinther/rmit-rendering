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

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Shader.hpp"
#include "assets/Texture.hpp"

#include "renderer/Material.hpp"

#include "Utilities.hpp"

GLResourceManager::GLResourceManager()
{
    // TODO Auto-generated constructor stub

}

GLResourceManager::~GLResourceManager()
{
    // TODO Auto-generated destructor stub
}

template<>
Model& GLResourceManager::bufferObject<Model>(Model& model, AssetManager& assetManager)
{
    if (nullptr == model.scene)
        throw std::runtime_error("Trying to render scene with NULL scene");

    using BufferIndices = Model::RenderInfo::MeshInfo::BufferIndices;
    auto& renderInfo = model.renderInfo;
    const auto& scene = *(model.scene);

    if (model.renderInfo.state & Model::RenderInfo::State::PRISTINE)
    {
        model.renderInfo.meshes.resize(scene.mNumMeshes);
        model.renderInfo.state ^= Model::RenderInfo::State::PRISTINE;

        for (unsigned int i = 0; i < scene.mNumMeshes; ++i)
        {
            model.renderInfo.meshes[i].numFaces = scene.mMeshes[i]->mNumFaces;
            model.renderInfo.meshes[i].numVertices = scene.mMeshes[i]->mNumVertices;
        }

        model.renderInfo.state |= Model::RenderInfo::State::DIRTY;
    }

    if (renderInfo.state & (Model::RenderInfo::State::DIRTY))
    {
        unsigned int meshIdx = 0;
        for (auto& mesh: model.renderInfo.meshes)
        {
            glDeleteBuffers(mesh.buffers.size(), mesh.buffers.data());
            glDeleteVertexArrays(1, &mesh.vao);

            glGenBuffers(mesh.buffers.size(), mesh.buffers.data());
            glGenVertexArrays(1, &mesh.vao);

            std::vector<GLuint> indices(mesh.numFaces * 3, 0);

            const auto& sceneMesh = *(scene.mMeshes[meshIdx]);
            for (unsigned int j = 0; j < mesh.numFaces; ++j)
            {
                std::copy(sceneMesh.mFaces[j].mIndices, sceneMesh.mFaces[j].mIndices + 3, indices.data() + j * 3);
            }

            glBindVertexArray(mesh.vao);

            glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_VBO]);
            glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), sceneMesh.mVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_NORMALS]);
            glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), sceneMesh.mNormals, GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_TANGENTS]);
            glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), sceneMesh.mTangents, GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_TEXCOORDS]);
            glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), sceneMesh.mTextureCoords[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_IBO]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.numFaces * 3 * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

            mesh.material = std::make_unique<Material>();
            mesh.material->createFromAssimpMaterial(*(scene.mMaterials[sceneMesh.mMaterialIndex]), model, assetManager);

            meshIdx += 1;
        }

        renderInfo.state = Model::RenderInfo::State::READY;

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    return model;
}

std::unique_ptr<Material> GLResourceManager::createMaterial(const aiMaterial& material, const Model& model, AssetManager& assetManager)
{
    auto result = std::make_unique<Material>();

    UNUSED(material);
    UNUSED(model);
    UNUSED(assetManager);

    return result;
}
