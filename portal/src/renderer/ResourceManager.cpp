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

#include "renderer/BufferedModel.hpp"
#include "renderer/BufferedTexture.hpp"
#include "renderer/BufferedMaterial.hpp"
#include "renderer/BufferedShader.hpp"

#include "Utilities.hpp"

renderer::ResourceManager::ResourceManager()
    : bufferedModels()
{
}

renderer::ResourceManager::~ResourceManager()
{
}

template <class T, class CacheType>
T& fetchFromCache(std::shared_ptr<const typename T::asset_type>& asset, CacheType& cache, renderer::ResourceManager& resourceManager)
{
    const auto& bufferedObject = cache.find(asset->hash);

    if (std::end(cache) != bufferedObject)
    {
        return *(cache[asset->hash]);
    } else
    {
        auto newBufferedObject = std::make_unique<T>(asset);
        newBufferedObject->loadFromAsset(resourceManager);

        cache[asset->hash] = std::move(newBufferedObject);

        return *(cache[asset->hash]);
    }
}

void renderer::ResourceManager::updateBuffers()
{
	for (const auto& bufferedAsset: bufferedShaders)
		if (!bufferedAsset.second->checkAssetVersion())
			bufferedAsset.second->loadFromAsset(*this);
}

namespace renderer
{

template <>
const BufferedModel& ResourceManager::getByAsset<BufferedModel>(
        std::shared_ptr<const typename BufferedModel::asset_type> asset)
{
    return fetchFromCache<BufferedModel>(asset, bufferedModels, *this);
}

template <>
const BufferedTexture& ResourceManager::getByAsset<BufferedTexture>(
        std::shared_ptr<const typename BufferedTexture::asset_type> asset)
{
    return fetchFromCache<BufferedTexture>(asset, bufferedTextures, *this);
}

template <>
const BufferedMaterial& ResourceManager::getByAsset<BufferedMaterial>(
        std::shared_ptr<const typename BufferedMaterial::asset_type> asset)
{
    return fetchFromCache<BufferedMaterial>(asset, bufferedMaterials, *this);
}

template <>
const BufferedShader& ResourceManager::getByAsset<BufferedShader>(
        std::shared_ptr<const typename BufferedShader::asset_type> asset)
{
    return fetchFromCache<BufferedShader>(asset, bufferedShaders, *this);
}

template<>
const BufferedModel& ResourceManager::getByHash<BufferedModel>(size_t hash)
{
    return *bufferedModels.at(hash);
}

template<>
const BufferedTexture& ResourceManager::getByHash<BufferedTexture>(size_t hash)
{
    return *bufferedTextures.at(hash);
}

template<>
const BufferedMaterial& ResourceManager::getByHash<BufferedMaterial>(size_t hash)
{
    return *bufferedMaterials.at(hash);
}

template<>
const BufferedShader& ResourceManager::getByHash<BufferedShader>(size_t hash)
{
    return *bufferedShaders.at(hash);
}




}


