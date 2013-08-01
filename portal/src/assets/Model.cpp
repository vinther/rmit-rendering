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

#include "renderer/Material.hpp"

#include "Utilities.hpp"

Model::Model(const std::string& name)
    : Asset(name, Type::TYPE_MODEL)
    , renderInfo()
    , importer()
    , scene()
{
}

Model::~Model()
{
    // TODO Auto-generated destructor stub
}

bool Model::loadFromDisk()
{
    auto scene = importer.ReadFile("assets/" + name,
            aiProcess_CalcTangentSpace      |
            aiProcess_GenNormals            |
            aiProcess_JoinIdenticalVertices |
            aiProcess_Triangulate           |
            aiProcess_GenUVCoords           |
            aiProcess_SortByPType);

    if (scene)
    {
        this->scene = decltype(this->scene)(scene, [](const aiScene*){});

        for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
        {
//            const auto& material = *(scene->mMaterials[i]);

//            importer.GetPropertyString()
//
//            aiString s;
//            material.Get(AI_MATKEY_TEXTURE_AMBIENT(0), s);
//            SDL_Log("Model \"%s\" has materials?: %s", name.c_str(), s.C_Str());
        }

        files.clear();
        files.push_back("assets/" + name);

        return true;
    }

    return false;
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
    loadFromDisk();

    renderInfo.state |= RenderInfo::DIRTY;
}

Model::RenderInfo::RenderInfo()
    : vbo(), vao(), ibo()
    , normals(), texCoords(), tangents()
    , numFaces(), material()
{
}
