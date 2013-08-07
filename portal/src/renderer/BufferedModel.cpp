/*
 * GLBufferedModel.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#include "renderer/BufferedModel.hpp"

#include <stdexcept>
#include <unordered_map>

#include <GL/glew.h>
#include <GL/gl.h>

#include <assimp/scene.h>

#include "renderer/ResourceManager.hpp"
#include "renderer/BufferedMaterial.hpp"
#include "Utilities.hpp"

struct MeshData
{
    struct SubMesh
    {
        unsigned int numVertices, numFaces;
        std::array<void*,  4> pointers;

        std::vector<GLuint> indices;
    };

    std::vector<SubMesh> subMeshes;

    unsigned int totalNumVertices;
    unsigned int totalNumFaces;
    unsigned int materialId;
};

renderer::BufferedModel::BufferedModel()
    : BufferObject()
{
}

renderer::BufferedModel::MeshInfo::MeshInfo()
    : buffers{{0, 0, 0, 0, 0}}
    , materialHash(0)
    , vao(0)
    , numVertices(0), numFaces(0)
{
}

renderer::BufferedModel::MeshInfo::MeshInfo(MeshInfo&& other)
    : buffers(std::move(other.buffers))
    , materialHash(other.materialHash)
    , vao(other.vao)
    , numVertices(other.numVertices), numFaces(other.numVertices)
{
}

renderer::BufferedModel::MeshInfo& renderer::BufferedModel::MeshInfo::operator=(MeshInfo&& other)
{
    buffers = std::move(other.buffers);
    materialHash = std::move(other.materialHash);
    vao = other.vao;
    numVertices = other.numVertices; numFaces = other.numVertices;

    return *this;
}

void renderer::BufferedModel::loadFromAsset(std::shared_ptr<const asset_type>& asset, ResourceManager& resourceManager)
{
    if (nullptr == asset->scene)
       throw std::runtime_error("Trying to render scene with NULL scene");

    typedef BufferedModel::MeshInfo::BufferIndices BufferIndices;
    const auto& scene = *(asset->scene);

   for (const auto& mesh: meshes)
   {
       glDeleteBuffers(mesh.buffers.size(), mesh.buffers.data());
       glDeleteVertexArrays(1, &mesh.vao);
   }

   for (const auto& materialPtr: asset->materials)
   {
       std::shared_ptr<const assets::Material> d = materialPtr;

       resourceManager.getByAsset<BufferedMaterial>(d);
   }

   std::unordered_map<unsigned int, MeshData> test;
   for (unsigned int i = 0; i < scene.mNumMeshes; ++i)
   {
       const auto& mesh = *(scene.mMeshes[i]);
       auto& tempMesh = test[mesh.mMaterialIndex];

       auto subMesh = MeshData::SubMesh{
           mesh.mNumVertices,
           mesh.mNumFaces,
           {{mesh.mVertices, mesh.mNormals, mesh.mTangents, mesh.mTextureCoords[0]}},
           {}
       };

       subMesh.indices.resize(mesh.mNumFaces * 3);

       for (unsigned int j = 0; j < mesh.mNumFaces; ++j)
       {
           std::copy(mesh.mFaces[j].mIndices, mesh.mFaces[j].mIndices + 3, subMesh.indices.data() + j * 3);
       }

       for (auto& index: subMesh.indices)
       {
           index += tempMesh.totalNumVertices;
       }

       tempMesh.subMeshes.push_back(std::move(subMesh));
       tempMesh.totalNumVertices += mesh.mNumVertices;
       tempMesh.totalNumFaces += mesh.mNumFaces;
   }

   for (const auto& tempMeshIterator: test)
   {
       unsigned int materialIndex = tempMeshIterator.first;
       const MeshData& tempMesh = tempMeshIterator.second;

       MeshInfo mesh;

       mesh.materialHash = asset->materials.at(materialIndex)->hash;
       mesh.numVertices = tempMesh.totalNumVertices;
       mesh.numFaces = tempMesh.totalNumFaces;

       glGenVertexArrays(1, &mesh.vao);
       glBindVertexArray(mesh.vao);

       glGenBuffers(mesh.buffers.size(), mesh.buffers.data());

       size_t offset = 0;

       /* Upload vertex data */
       glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_VBO]);
       glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), 0, GL_STATIC_DRAW);

       offset = 0;
       for (const auto& subMesh: tempMesh.subMeshes)
       {
           glBufferSubData(GL_ARRAY_BUFFER, offset, subMesh.numVertices * sizeof(aiVector3D), subMesh.pointers[BufferIndices::BUFFER_VBO]);
           offset += subMesh.numVertices * sizeof(aiVector3D);
       }

       glEnableVertexAttribArray(BufferIndices::BUFFER_VBO);
       glVertexAttribPointer(BufferIndices::BUFFER_VBO, 3, GL_FLOAT, GL_FALSE, 0, 0);

       /* Upload normal data */
       glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_NORMALS]);
       glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), 0, GL_STATIC_DRAW);

       offset = 0;
       for (const auto& subMesh: tempMesh.subMeshes)
       {
           glBufferSubData(GL_ARRAY_BUFFER, offset, subMesh.numVertices * sizeof(aiVector3D), subMesh.pointers[BufferIndices::BUFFER_NORMALS]);
           offset += subMesh.numVertices * sizeof(aiVector3D);
       }

       glEnableVertexAttribArray(BufferIndices::BUFFER_NORMALS);
       glVertexAttribPointer(BufferIndices::BUFFER_NORMALS, 3, GL_FLOAT, GL_FALSE, 0, 0);

       /* Upload tangent data */
       glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_TANGENTS]);
       glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), 0, GL_STATIC_DRAW);

       offset = 0;
       for (const auto& subMesh: tempMesh.subMeshes)
       {
           glBufferSubData(GL_ARRAY_BUFFER, offset, subMesh.numVertices * sizeof(aiVector3D), subMesh.pointers[BufferIndices::BUFFER_TANGENTS]);
           offset += subMesh.numVertices * sizeof(aiVector3D);
       }

       glEnableVertexAttribArray(BufferIndices::BUFFER_TANGENTS);
       glVertexAttribPointer(BufferIndices::BUFFER_TANGENTS, 3, GL_FLOAT, GL_FALSE, 0, 0);

       /* Upload tex-coords data */
       glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_TEXCOORDS]);
       glBufferData(GL_ARRAY_BUFFER, mesh.numVertices * sizeof(aiVector3D), 0, GL_STATIC_DRAW);

       offset = 0;
       for (const auto& subMesh: tempMesh.subMeshes)
       {
           glBufferSubData(GL_ARRAY_BUFFER, offset, subMesh.numVertices * sizeof(aiVector3D), subMesh.pointers[BufferIndices::BUFFER_TEXCOORDS]);
           offset += subMesh.numVertices * sizeof(aiVector3D);
       }

       glEnableVertexAttribArray(BufferIndices::BUFFER_TEXCOORDS);
       glVertexAttribPointer(BufferIndices::BUFFER_TEXCOORDS, 3, GL_FLOAT, GL_FALSE, 0, 0);

       /* Upload index data */
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[BufferIndices::BUFFER_IBO]);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.numFaces * 3 * sizeof(GLuint), 0, GL_STATIC_DRAW);

       offset = 0;
       for (const auto& subMesh: tempMesh.subMeshes)
       {
           glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, subMesh.indices.size() * sizeof(GLuint), subMesh.indices.data());
           offset += subMesh.indices.size() * sizeof(GLuint);
       }

       glBindVertexArray(0);
       glBindBuffer(GL_ARRAY_BUFFER, 0);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

       meshes.push_back(std::move(mesh));
   }

//   const auto sorter = [](const MeshInfo& meshA, const MeshInfo& meshB)
//   {
//       return (meshA.material->name < meshB.material->name);
//   };

//   std::sort(meshes.begin(), meshes.end(), sorter);

   state = State::READY;
}
