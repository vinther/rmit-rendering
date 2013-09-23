/*
 * GLResourceManager.hpp
 *
 *  Created on: 01/08/2013
 *      Author: svp
 */

#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <memory>
#include <unordered_map>

#include "shared/utilities.hpp"
#include "shared/handle.hpp"

namespace renderer
{

namespace resources
{
class Model;
class Texture;
class Material;
class ShaderProgram;
class FrameBuffer;
}

typedef Handle<resources::Model> ModelHandle;
typedef Handle<resources::Texture> TextureHandle;
typedef Handle<resources::Material> MaterialHandle;
typedef Handle<resources::ShaderProgram> ShaderProgramHandle;
typedef Handle<resources::FrameBuffer> FrameBufferHandle;

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    template <class T>
    const std::shared_ptr<T> getByAsset(std::shared_ptr<const typename T::asset_type> asset);

    template <class T>
    const std::shared_ptr<T> getByHash(size_t hash);

    void updateResources();

    std::unordered_map<size_t, std::shared_ptr<resources::Model>> models;
    std::unordered_map<size_t, std::shared_ptr<resources::Texture>> textures;
    std::unordered_map<size_t, std::shared_ptr<resources::Material>> materials;
    std::unordered_map<size_t, std::shared_ptr<resources::ShaderProgram>> shaders;
};

}

#endif /* RESOURCEMANAGER_HPP_ */
