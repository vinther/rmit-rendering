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

class aiScene;
class AssetManager;

class Model: public Asset
{
public:
    Model(const std::string& name);
    virtual ~Model();

    struct RenderInformation
    {
        enum State
        {
            PRISTINE    = 0x01,
            BUFFERED    = 0x02,
            DIRTY       = 0x04,
        };

        std::vector<GLuint> vbo, ibo, vao;
        std::vector<GLuint> numFaces;

        Uint16 state = State::PRISTINE;
    } renderInformation;

    bool loadFromDisk(const std::string& name, AssetManager& assetManager);
    size_t reportSize() const;

    Assimp::Importer importer;
    const aiScene* scene;
};

#endif /* MODEL_HPP_ */
