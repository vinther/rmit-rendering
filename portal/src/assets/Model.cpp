/*
 * Model.cpp
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

        files.clear();
        files.push_back(name);

        path = name;

#ifdef linux
        auto pathCopy = path;
        basePath = std::string(dirname((char*)pathCopy.c_str()));
#endif

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

Model::RenderInfo::MeshInfo::MeshInfo()
    : buffers{{0, 0, 0, 0, 0}}
    , material()
    , vao(0)
    , numVertices(0), numFaces(0)
{
}

Model::RenderInfo::MeshInfo::MeshInfo(Model::RenderInfo::MeshInfo&& other)
    : buffers(std::move(other.buffers))
    , material(std::move(other.material))
    , vao(other.vao)
    , numVertices(other.numVertices), numFaces(other.numVertices)
{
}

Model::RenderInfo::MeshInfo& Model::RenderInfo::MeshInfo::operator=(Model::RenderInfo::MeshInfo&& other)
{
    buffers = std::move(other.buffers);
    material = std::move(other.material);
    vao = other.vao;
    numVertices = other.numVertices; numFaces = other.numVertices;

    return *this;
}


