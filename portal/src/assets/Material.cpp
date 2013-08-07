/*
 * Material.cpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#include "assets/Material.hpp"

#include <assimp/material.h>

#include "assets/AssetManager.hpp"
#include "assets/Texture.hpp"

assets::Material::Material(const std::string& name)
    : Asset(name, Asset::Type::TYPE_MATERIAL)
    , translucent(false)
{
}

assets::Material::~Material()
{
}

bool assets::Material::loadFromDisk(const std::string& basePath, const aiMaterial& material, AssetManager& assetManager)
{
    aiString ambientTexture, diffuseTexture, specularTexture, normalsTexture, heightTexture, opacityTexture;
    material.GetTexture(aiTextureType_AMBIENT, 0, &ambientTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_SPECULAR, 0, &specularTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_NORMALS, 0, &normalsTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_HEIGHT, 0, &heightTexture, nullptr, nullptr, nullptr, nullptr);
    material.GetTexture(aiTextureType_OPACITY, 0, &opacityTexture, nullptr, nullptr, nullptr, nullptr);

    if (0 < ambientTexture.length)
        texAmbient = assetManager.getOrCreate<Texture>(std::string(ambientTexture.C_Str()), basePath + std::string(ambientTexture.C_Str()));

    if (0 < diffuseTexture.length)
        texDiffuse = assetManager.getOrCreate<Texture>(std::string(diffuseTexture.C_Str()), basePath + std::string(diffuseTexture.C_Str()));

    if (0 < specularTexture.length)
        texSpecular = assetManager.getOrCreate<Texture>(std::string(specularTexture.C_Str()), basePath + std::string(specularTexture.C_Str()));

    if (0 < normalsTexture.length)
        texNormal = assetManager.getOrCreate<Texture>(std::string(normalsTexture.C_Str()), basePath + std::string(normalsTexture.C_Str()));

    if (0 < heightTexture.length)
        texBump = assetManager.getOrCreate<Texture>(std::string(heightTexture.C_Str()), basePath + std::string(heightTexture.C_Str()));

    aiColor3D emissive, ambient, diffuse, specular;
    float shininess;

    material.Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    material.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    material.Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material.Get(AI_MATKEY_SHININESS, shininess);

    materialInfo.emission = glm::vec4(emissive.r, emissive.g, emissive.b, 1.0f);
    materialInfo.ambient = glm::vec4(ambient.r, ambient.g, ambient.b, 1.0f);
    materialInfo.diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
    materialInfo.specular = glm::vec4(specular.r, specular.g, specular.b, 1.0f);
    materialInfo.shininess = shininess;

    if (texDiffuse && texDiffuse->surface)
        translucent = texDiffuse->surface->format->BytesPerPixel == 4;

    return true;
}

size_t assets::Material::reportSize() const
{
    return 0;
}

void assets::Material::reload()
{
}
