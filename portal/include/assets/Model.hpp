/*
 * Model.hpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "assets/Asset.hpp"

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <assimp/Importer.hpp>

#include "renderer/RenderInfoBase.hpp"

class aiScene;
class AssetManager;

class Model: public Asset
{
public:
    Model(const std::string& name);
    virtual ~Model();

    struct RenderInfo: public RenderInfoBase
    {
        std::vector<GLuint> vbo, vao, ibo;
        std::vector<GLuint> normals, texCoords, tangents;
        std::vector<GLuint> numFaces;
    } renderInfo;

    bool loadFromDisk(const std::string& path);
    size_t reportSize() const;
    void reload();

    Assimp::Importer importer;
    const aiScene* scene;
};

#endif /* MODEL_HPP_ */
