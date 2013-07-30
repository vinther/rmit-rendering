/*
 * Model.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "assets/Model.hpp"

#include <iostream>
#include <stdexcept>

#include <assimp/scene.h>

Model::Model(const std::string& name)
    : Asset(name, Type::TYPE_MODEL)
{
}

Model::~Model()
{
    // TODO Auto-generated destructor stub
}

void Model::initialize(const aiScene* scene, AssetManager& assetManager)
{
    this->scene = scene;

    if (!scene)
        throw std::runtime_error("Something is very wrong");

    aiString s;
    aiMaterial* d = scene->mMaterials[scene->mMeshes[0]->mMaterialIndex];

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        d->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT,0), s);
        SDL_Log("Model \"%s\" has materials?: %s", name.c_str(), s.C_Str());
    }

}
