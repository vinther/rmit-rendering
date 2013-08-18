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

#include "Utilities.hpp"

namespace renderer
{

class BufferedModel;
class BufferedTexture;
class BufferedMaterial;
class BufferedShader;

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    template <class T>
    const T& getByAsset(std::shared_ptr<const typename T::asset_type> asset);

    template <class T>
    const T& getByHash(size_t hash);

    void updateBuffers();

    std::unordered_map<size_t, std::unique_ptr<BufferedModel>> bufferedModels;
    std::unordered_map<size_t, std::unique_ptr<BufferedTexture>> bufferedTextures;
    std::unordered_map<size_t, std::unique_ptr<BufferedMaterial>> bufferedMaterials;
    std::unordered_map<size_t, std::unique_ptr<BufferedShader>> bufferedShaders;
};

}

#endif /* RESOURCEMANAGER_HPP_ */
