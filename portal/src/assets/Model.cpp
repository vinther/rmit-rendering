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
#include <assimp/postprocess.h>

#include "assets/AssetManager.hpp"

Model::Model(const std::string& name)
    : Asset(name, Type::TYPE_MODEL)
    , scene(nullptr)
{
}

Model::~Model()
{
    // TODO Auto-generated destructor stub
}

bool Model::loadFromDisk(const std::string& name, AssetManager& assetManager)
{
    scene = importer.ReadFile("assets/" + name,
            aiProcess_CalcTangentSpace       |
            aiProcess_Triangulate            |
            aiProcess_JoinIdenticalVertices  |
            aiProcess_SortByPType);

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
//        const auto& material = *(scene->mMaterials[i]);
//
//        assetManager.get<Model>("TEST");
//        d->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT,0), s);
//        SDL_Log("Model \"%s\" has materials?: %s", name.c_str(), s.C_Str());
    }

    return (scene);
}

size_t Model::reportSize() const
{
    aiMemoryInfo memoryInfo;
    importer.GetMemoryRequirements(memoryInfo);

    return memoryInfo.total;
}
