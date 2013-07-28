/*
 * AssetManager.hpp
 *
 *  Created on: 28/07/2013
 *      Author: svp
 */

#ifndef ASSETMANAGER_HPP_
#define ASSETMANAGER_HPP_

#include <type_traits>
#include <memory>
#include <unordered_map>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "assets/Asset.hpp"

class AssetManager
{
public:
    template <class T>
    std::weak_ptr<T> get(const std::string& name);

private:
    Assimp::Importer importer;

    std::unordered_map<std::string, std::shared_ptr<Asset>> cachedAssets;
};


// assetManager->get<Model>("models/aeroplane.obj");

#endif /* ASSETMANAGER_HPP_ */
