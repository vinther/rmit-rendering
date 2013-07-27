/*
 * Scene.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "Scene.hpp"


#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <iostream>

Scene::Scene()
    : camera(std::make_shared<Camera>())
{
    // TODO Auto-generated constructor stub

}

Scene::~Scene()
{
    // TODO Auto-generated destructor stub
}

void Scene::loadObject(const std::string& path)
{
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( path,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

    (void)(scene);

    std::cout << scene->mNumMeshes << std::endl;

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        std::cout << scene->mMeshes[i]->mNumVertices << std::endl;
    }
}
