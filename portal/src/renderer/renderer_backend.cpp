#include <stdexcept>
#include <array>
#include <tuple>

#include <GL/glew.h>
#include <GL/gl.h>
#include <assimp/scene.h>

#include "scene/scene_graph.hpp"

#include "assets/asset_store.hpp"
#include "assets/scene.hpp"
#include "assets/image.hpp"

#include "renderer/resources/Model.hpp"
#include "renderer/resources/Texture.hpp"
#include "renderer/resources/Material.hpp"
#include "renderer/resources/ShaderProgram.hpp"
#include "renderer/resources/FrameBuffer.hpp"

#include "Utilities.hpp"

renderer::storage_backend::storage_backend()
    : models()
{
//    FrameBufferHandle h1;
//    TextureHandle h2;
//    FrameBufferHandle h3;
//
//    h1 = h3;

}

renderer::storage_backend::~storage_backend()
{
}

template <class T, class CacheType>
std::shared_ptr<T>& fetchFromCache(std::shared_ptr<const typename T::asset_type>& asset, CacheType& cache, renderer::storage_backend& resourceManager)
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

void renderer::storage_backend::updateResources()
{
//	for (const auto& resource: shaders)
//		if (!resource.second->checkAssetVersion())
//		    resource.second->loadFromAsset(*this);
}

namespace renderer
{

template <>
const std::shared_ptr<resources::Mesh> storage_backend::getByAsset<resources::Mesh>(
        std::shared_ptr<const typename resources::Mesh::asset_type> asset)
{
    return fetchFromCache<resources::Mesh>(asset, models, *this);
}

template <>
const std::shared_ptr<resources::Texture> storage_backend::getByAsset<resources::Texture>(
        std::shared_ptr<const typename resources::Texture::asset_type> asset)
{
    return fetchFromCache<resources::Texture>(asset, textures, *this);
}

template <>
const std::shared_ptr<resources::Material> storage_backend::getByAsset<resources::Material>(
        std::shared_ptr<const typename resources::Material::asset_type> asset)
{
    return fetchFromCache<resources::Material>(asset, materials, *this);
}

template<>
const std::shared_ptr<resources::Mesh> storage_backend::getByHash<resources::Mesh>(size_t hash)
{
    return models.at(hash);
}

template<>
const std::shared_ptr<resources::Texture> storage_backend::getByHash<resources::Texture>(size_t hash)
{
    return textures.at(hash);
}

template<>
const std::shared_ptr<resources::Material> storage_backend::getByHash<resources::Material>(size_t hash)
{
    return materials.at(hash);
}

template<>
const std::shared_ptr<resources::ShaderProgram> storage_backend::getByHash<resources::ShaderProgram>(size_t hash)
{
    return shaders.at(hash);
}




}


