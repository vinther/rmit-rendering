/*
 * GLResourceManager.cpp
 *
 *  Created on: 01/08/2013
 *      Author: svp
 */

#include "renderer/ResourceManager.hpp"

#include <stdexcept>
#include <array>
#include <tuple>

#include <GL/glew.h>
#include <GL/gl.h>
#include <assimp/scene.h>

#include "scene/Scene.hpp"

#include "assets/AssetManager.hpp"
#include "assets/Model.hpp"
#include "assets/Shader.hpp"
#include "assets/Texture.hpp"

#include "renderer/resources/Model.hpp"
#include "renderer/resources/Texture.hpp"
#include "renderer/resources/Material.hpp"
#include "renderer/resources/Shader.hpp"
#include "renderer/resources/FrameBuffer.hpp"

#include "Utilities.hpp"

renderer::ResourceManager::ResourceManager()
    : models()
{
}

renderer::ResourceManager::~ResourceManager()
{
}

template <class T, class CacheType>
std::shared_ptr<T>& fetchFromCache(std::shared_ptr<const typename T::asset_type>& asset, CacheType& cache, renderer::ResourceManager& resourceManager)
{
    const auto& bufferedObject = cache.find(asset->hash);

    if (std::end(cache) != bufferedObject)
    {
        return cache[asset->hash];
    } else
    {
        auto newBufferedObject = std::make_unique<T>(asset);
        newBufferedObject->loadFromAsset(resourceManager);

        cache[asset->hash] = std::move(newBufferedObject);

        return cache[asset->hash];
    }
}

void renderer::ResourceManager::updateResources()
{
	for (const auto& resource: shaders)
		if (!resource.second->checkAssetVersion())
		    resource.second->loadFromAsset(*this);
}

namespace renderer
{

template <>
const std::shared_ptr<resources::Model> ResourceManager::getByAsset<resources::Model>(
        std::shared_ptr<const typename resources::Model::asset_type> asset)
{
    return fetchFromCache<resources::Model>(asset, models, *this);
}

template <>
const std::shared_ptr<resources::Texture> ResourceManager::getByAsset<resources::Texture>(
        std::shared_ptr<const typename resources::Texture::asset_type> asset)
{
    return fetchFromCache<resources::Texture>(asset, textures, *this);
}

template <>
const std::shared_ptr<resources::Material> ResourceManager::getByAsset<resources::Material>(
        std::shared_ptr<const typename resources::Material::asset_type> asset)
{
    return fetchFromCache<resources::Material>(asset, materials, *this);
}

template <>
const std::shared_ptr<resources::ShaderProgram> ResourceManager::getByAsset<resources::ShaderProgram>(
        std::shared_ptr<const typename resources::ShaderProgram::asset_type> asset)
{
    return fetchFromCache<resources::ShaderProgram>(asset, shaders, *this);
}

template<>
const std::shared_ptr<resources::Model> ResourceManager::getByHash<resources::Model>(size_t hash)
{
    return models.at(hash);
}

template<>
const std::shared_ptr<resources::Texture> ResourceManager::getByHash<resources::Texture>(size_t hash)
{
    return textures.at(hash);
}

template<>
const std::shared_ptr<resources::Material> ResourceManager::getByHash<resources::Material>(size_t hash)
{
    return materials.at(hash);
}

template<>
const std::shared_ptr<resources::ShaderProgram> ResourceManager::getByHash<resources::ShaderProgram>(size_t hash)
{
    return shaders.at(hash);
}




}


