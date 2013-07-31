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

bool Model::loadFromDisk(const std::string& name)
{
    auto tempScene = importer.ReadFile("assets/" + name,
            aiProcess_CalcTangentSpace      |
            aiProcess_GenNormals            |
            aiProcess_JoinIdenticalVertices |
            aiProcess_Triangulate           |
            aiProcess_GenUVCoords           |
            aiProcess_SortByPType);

//    for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
//    {
//        const auto& material = *(scene->mMaterials[i]);
//
//        assetManager.get<Model>("TEST");
//        d->Get(AI_MATKEY_TEXTURE(aiTextureType_HEIGHT,0), s);
//        SDL_Log("Model \"%s\" has materials?: %s", name.c_str(), s.C_Str());
//    }

    if (tempScene)
    {
        scene = tempScene;

        files.clear();
        files.push_back("assets/" + name);
    }

    return (scene);
}

size_t Model::reportSize() const
{
    aiMemoryInfo memoryInfo;
    importer.GetMemoryRequirements(memoryInfo);

    return memoryInfo.total;
}

void Model::reload()
{
    importer.FreeScene();
    loadFromDisk(name);

    renderInfo.state |= RenderInfo::DIRTY;
}
