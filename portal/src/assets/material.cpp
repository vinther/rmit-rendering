/*
 * Material.cpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#include "assets/material.hpp"

#include <assimp/material.h>

#include "assets/asset_store.hpp"
#include "assets/image.hpp"

assets::material::material(const std::string& name)
    : asset(name, asset::Type::TYPE_MATERIAL)
    , translucent(false)
{
}

assets::material::~material()
{
}

bool assets::material::loadFromDisk(const std::string& basePath, const aiMaterial& material, asset_store& assetManager)
{
	for (const auto& pair: {
			std::make_pair(0u, aiTextureType_EMISSIVE),
			std::make_pair(1u, aiTextureType_AMBIENT),
			std::make_pair(2u, aiTextureType_DIFFUSE),
			std::make_pair(3u, aiTextureType_SPECULAR),
			std::make_pair(4u, aiTextureType_HEIGHT)})
	{
		aiString texturePath;

		material.GetTexture(pair.second, 0, &texturePath, nullptr, nullptr, nullptr, nullptr);

		const std::string fullPath = basePath + std::string(texturePath.C_Str());

		if (0 < texturePath.length)
			textures[pair.first] =
					assetManager.create<image>(std::string(texturePath.C_Str()), fullPath.c_str());
	}

    aiColor3D emissive, ambient, diffuse, specular;
    float shininess;

    material.Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
    material.Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    material.Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    material.Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material.Get(AI_MATKEY_SHININESS, shininess);

    this->emission = glm::vec4(emissive.r, emissive.g, emissive.b, 1.0f);
    this->ambient = glm::vec4(ambient.r, ambient.g, ambient.b, 1.0f);
    this->diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
    this->specular = glm::vec4(specular.r, specular.g, specular.b, 1.0f);
    this->shininess = shininess;

    if (textures[2] && assetManager(textures[2]).surface)
        translucent = assetManager(textures[2]).surface->format->BytesPerPixel == 4;

    return true;
}

size_t assets::material::reportSize() const
{
    return 0;
}

void assets::material::reload()
{
}
