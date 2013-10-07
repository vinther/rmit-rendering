/*
 * PointLightGroup.cpp
 *
 *  Created on: 26/08/2013
 *      Author: svp
 */

#include "renderer/resources/PointLightGroup.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include "assimp/scene.h"

#include "Utilities.hpp"

renderer::resources::PointLightGroup::PointLightGroup()
    : meshData({0, 0})
//    , buffer(std::make_unique<UniformBuffer>())
//    , count(0)
{
    glGenVertexArrays(1, &meshData.vao);
}

renderer::resources::PointLightGroup::PointLightGroup(
        const std::shared_ptr<const assets::Model> geometry,
        const std::initializer_list<LightData>& data)
    : PointLightGroup()
{
    create(geometry, data);
}

renderer::resources::PointLightGroup::~PointLightGroup()
{
    glDeleteVertexArrays(1, &meshData.vao);
}

void renderer::resources::PointLightGroup::create(
        const std::shared_ptr<const assets::Model> geometry,
        const std::vector<LightData>& data)
{
//    buffer->enable();
//    buffer->data(data.size() * sizeof(LightData), data.data());
//    buffer->disable();

    glBindVertexArray(meshData.vao);

    GLuint buffers[2];
    glGenBuffers(2, buffers);

    /* Upload vertex data */
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER,
           geometry->scene->mMeshes[0]->mNumVertices * sizeof(aiVector3D),
           geometry->scene->mMeshes[0]->mVertices,
           GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* Upload index data */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            geometry->scene->mMeshes[0]->mNumFaces * 3 * sizeof(GLuint),
            0, GL_STATIC_DRAW);

    size_t offset = 0;
    for (unsigned int i = 0; i < geometry->scene->mMeshes[0]->mNumFaces; ++i)
    {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset,
                geometry->scene->mMeshes[0]->mFaces[i].mNumIndices * sizeof(GLuint),
                geometry->scene->mMeshes[0]->mFaces[i].mIndices);

        offset += geometry->scene->mMeshes[0]->mFaces[i].mNumIndices * sizeof(GLuint);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    this->data = data;

    meshData.numFaces = geometry->scene->mMeshes[0]->mNumFaces;
}
