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
        RenderInfo();

        std::vector<GLuint> vbo, vao, ibo;
        std::vector<GLuint> normals, texCoords, tangents;
        std::vector<GLuint> numFaces;
        std::unique_ptr<Material> material;
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
};

#endif /* MODEL_HPP_ */
