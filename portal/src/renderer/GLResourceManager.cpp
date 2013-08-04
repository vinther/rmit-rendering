/*
 * GLResourceManager.cpp
 *
 *  Created on: 01/08/2013
 *      Author: svp
 */

#include "renderer/GLResourceManager.hpp"

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

#include "renderer/GLBufferedModel.hpp"
#include "renderer/GLBufferedTexture.hpp"
#include "renderer/GLBufferedMaterial.hpp"
#include "renderer/GLBufferedShader.hpp"

#include "Utilities.hpp"

GLResourceManager::GLResourceManager()
    : bufferedModels()
{
    // TODO Auto-generated constructor stub

}

GLResourceManager::~GLResourceManager()
{
    // TODO Auto-generated destructor stub
}

template <class T, class CacheType>
T& fetchFromCache(std::shared_ptr<const typename T::asset_type>& asset, CacheType& cache, GLResourceManager& resourceManager)
{
    const auto& bufferedObject = cache.find(asset->hash);

    if (std::end(cache) != bufferedObject)
    {
        return *(cache[asset->hash]);
    } else
    {
        auto newBufferedObject = std::make_unique<T>();
        newBufferedObject->loadFromAsset(asset, resourceManager);

        cache[asset->hash] = std::move(newBufferedObject);

        return *(cache[asset->hash]);
    }
}

template <>
const GLBufferedModel& GLResourceManager::getByAsset<GLBufferedModel>(std::shared_ptr<const typename GLBufferedModel::asset_type> asset)
{
    return fetchFromCache<GLBufferedModel>(asset, bufferedModels, *this);
}

template <>
const GLBufferedTexture& GLResourceManager::getByAsset<GLBufferedTexture>(std::shared_ptr<const typename GLBufferedTexture::asset_type> asset)
{
    return fetchFromCache<GLBufferedTexture>(asset, bufferedTextures, *this);
}

template <>
const GLBufferedMaterial& GLResourceManager::getByAsset<GLBufferedMaterial>(std::shared_ptr<const typename GLBufferedMaterial::asset_type> asset)
{
    return fetchFromCache<GLBufferedMaterial>(asset, bufferedMaterials, *this);
}

template <>
const GLBufferedShader& GLResourceManager::getByAsset<GLBufferedShader>(std::shared_ptr<const typename GLBufferedShader::asset_type> asset)
{
    return fetchFromCache<GLBufferedShader>(asset, bufferedShaders, *this);
}

template<>
const GLBufferedModel& GLResourceManager::getByHash<GLBufferedModel>(size_t hash)
{
    return *bufferedModels.at(hash);
}

template<>
const GLBufferedTexture& GLResourceManager::getByHash<GLBufferedTexture>(size_t hash)
{
    return *bufferedTextures.at(hash);
}

template<>
const GLBufferedMaterial& GLResourceManager::getByHash<GLBufferedMaterial>(size_t hash)
{
    return *bufferedMaterials.at(hash);
}

template<>
const GLBufferedShader& GLResourceManager::getByHash<GLBufferedShader>(size_t hash)
{
    return *bufferedShaders.at(hash);
}
