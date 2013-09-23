#include "assets/scene.hpp"

#include <iostream>
#include <stdexcept>

#ifdef linux
#include <libgen.h>
#endif

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assets/asset_store.hpp"
#include "assets/material.hpp"

#include "shared/utilities.hpp"

assets::scene::scene(const std::string& name)
    : assets::asset(name, Type::TYPE_MODEL)
    , assimp_scene()
	, importer()
{
}

assets::scene::~scene()
{
	importer.FreeScene();
}

bool assets::scene::loadFromDisk()
{
    auto temp_scene = importer.ReadFile("assets/" + identifier,
    		aiProcessPreset_TargetRealtime_Quality ^ (aiProcess_JoinIdenticalVertices | aiProcess_GenUVCoords) );

    if (temp_scene && !(temp_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        this->assimp_scene = decltype(this->assimp_scene)(temp_scene, [](const aiScene*){});

        assetFilePaths.clear();
        assetFilePaths.push_back(identifier);

        path = identifier;

        SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS, "Added model with %d meshes (%s)", temp_scene->mNumMeshes, identifier.c_str());

#ifdef linux
        const std::string pathCopy = path.c_str();
        basePath = std::string(dirname((char*)pathCopy.c_str())) + "/";

        assert(pathCopy.data() != path.data());
#endif

        return true;
    } else
    {
        SDL_LogError(client::PORTAL_LOG_CATEGORY_ASSETS, "Failed to load model: %s", identifier.c_str());
    }

    return false;
}

bool assets::scene::loadFromDisk(assets::asset_store& dataStore)
{
    if (!loadFromDisk())
        return false;

    materials.clear();
    for(unsigned int i = 0; i < assimp_scene->mNumMaterials; ++i)
    {
        const auto& material = *(assimp_scene->mMaterials[i]);
        aiString materialName;

        material.Get(AI_MATKEY_NAME, materialName);

        materials.push_back(dataStore.create<material>(materialName.C_Str(), basePath, std::ref(material), std::ref(dataStore)));
    }

    return true;
}

size_t assets::scene::reportSize() const
{
    aiMemoryInfo memoryInfo;
    importer.GetMemoryRequirements(memoryInfo);

    return memoryInfo.total;
}

void assets::scene::reload()
{
    SDL_LogDebug(client::PORTAL_LOG_CATEGORY_ASSETS, "Reloading model: \"%s\"", identifier.c_str());

    importer.FreeScene();
    loadFromDisk();
}




