/*
 * GLBufferedTexture.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef GLBUFFEREDMODEL_HPP_
#define GLBUFFEREDMODEL_HPP_

#include "renderer/GLBufferObject.hpp"

#include "assets/Model.hpp"

typedef unsigned int GLuint;

class GLResourceManager;

class GLBufferedModel: public GLBufferObject<assets::Model>
{
public:
    GLBufferedModel();

    struct MeshInfo
    {
        MeshInfo();
        MeshInfo(MeshInfo&& other);
        MeshInfo& operator=(MeshInfo&& other);

        enum BufferIndices
        {
            BUFFER_VBO = 0,
            BUFFER_NORMALS,
            BUFFER_TANGENTS,
            BUFFER_TEXCOORDS,
            BUFFER_IBO,

            BUFFER_LAST_INDEX
        };

        std::array<GLuint, BufferIndices::BUFFER_LAST_INDEX> buffers;
        size_t materialHash;

        GLuint vao;

        unsigned int numVertices;
        unsigned int numFaces;
    };

    std::vector<MeshInfo> meshes;

    void loadFromAsset(std::shared_ptr<const asset_type>& asset, GLResourceManager& resourceManager);
};

#endif /* GLBUFFEREDMODEL_HPP_ */
