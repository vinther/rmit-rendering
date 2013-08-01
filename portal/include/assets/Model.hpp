/*
 * Model.hpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "assets/Asset.hpp"

#include <memory>
#include <array>

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <assimp/Importer.hpp>

#include "renderer/RenderInfoBase.hpp"

class aiScene;
class AssetManager;
class Material;

class Model: public Asset
{
public:
    Model(const std::string& name);
    virtual ~Model();

    struct RenderInfo: public RenderInfoBase
    {
        struct MeshInfo
        {
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
            std::unique_ptr<Material> material;

            GLuint vao;

            unsigned int numFaces;
            unsigned int numVertices;
        };

        std::vector<MeshInfo> meshes;

    } renderInfo;

    struct SceneDeleter
    {
        constexpr SceneDeleter() = default;
        void operator()(const void*) {};
    };

    bool loadFromDisk();
    size_t reportSize() const override;
    void reload() override;

    Assimp::Importer importer;

    std::unique_ptr<const aiScene, std::function<void(const aiScene*)>> scene;

    std::string path;
    std::string basePath;
};

#endif /* MODEL_HPP_ */
