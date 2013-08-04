/*
 * GLResourceManager.hpp
 *
 *  Created on: 01/08/2013
 *      Author: svp
 */

#ifndef GLRESOURCEMANAGER_HPP_
#define GLRESOURCEMANAGER_HPP_

#include <memory>
#include <unordered_map>

#include "Utilities.hpp"

class GLBufferedModel;
class GLBufferedTexture;
class GLBufferedMaterial;
class GLBufferedShader;

class GLResourceManager
{
public:
    GLResourceManager();
    virtual ~GLResourceManager();

    template <class T>
    const T& getByAsset(std::shared_ptr<const typename T::asset_type> asset);

    template <class T>
    const T& getByHash(size_t hash);
private:
    std::unordered_map<size_t, std::unique_ptr<GLBufferedModel>> bufferedModels;
    std::unordered_map<size_t, std::unique_ptr<GLBufferedTexture>> bufferedTextures;
    std::unordered_map<size_t, std::unique_ptr<GLBufferedMaterial>> bufferedMaterials;
    std::unordered_map<size_t, std::unique_ptr<GLBufferedShader>> bufferedShaders;
};

#endif /* GLRESOURCEMANAGER_HPP_ */
