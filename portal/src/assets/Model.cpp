/*
R * Model.cpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#include "assets/Model.hpp"

#include <iostream>
#include <stdexcept>

#ifdef linux
#include <libgen.h>
#endif

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assets/AssetManager.hpp"
#include "assets/Material.hpp"

#include "Utilities.hpp"

assets::Model::Model(const std::string& name)
    : assets::Asset(name, Type::TYPE_MODEL)
    , scene()
	, importer()
{
}

assets::Model::~Model()
{
	importer.FreeScene();
}

bool assets::Model::loadFromDisk()
{
    auto scene = importer.ReadFile("assets/" + name,
    		aiProcessPreset_TargetRealtime_Quality ^ (aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords) );

    if (scene && !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        this->scene = decltype(this->scene)(scene, [](const aiScene*){});

        assetFilePaths.clear();
        assetFilePaths.push_back(name);

        path = name;

#ifdef linux
        auto pathCopy = path;
        basePath = std::string(dirname((char*)pathCopy.c_str())) + "/";
#endif

        //SDL_Log("Added model with %d meshes (%s)", scene->mNumMeshes, name.c_str());

        ++version;

        return true;
    }

    return false;
}

bool assets::Model::loadFromDisk(assets::AssetManager& assetManager)
{
    if (!loadFromDisk())
        return false;

    materials.clear();
    for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
    {
        const auto& material = *(scene->mMaterials[i]);
        aiString materialName;

        material.Get(AI_MATKEY_NAME, materialName);

        materials.push_back(assetManager.getOrCreate<Material>(materialName.C_Str(), basePath, std::ref(material), std::ref(assetManager)));
    }

    return true;
}

size_t assets::Model::reportSize() const
{
    aiMemoryInfo memoryInfo;
    importer.GetMemoryRequirements(memoryInfo);

    return memoryInfo.total;
}

void assets::Model::reload()
{
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS, "Reloading model: \"%s\"", name.c_str());

    importer.FreeScene();
    loadFromDisk();
}




