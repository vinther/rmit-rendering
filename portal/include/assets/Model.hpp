/*
 * Model.hpp
 *
 *  Created on: 30/07/2013
 *      Author: svp
 */

#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "assets/Asset.hpp"

#include <memory>
#include <array>

#include <SDL2/SDL.h>
#include <assimp/Importer.hpp>

struct aiScene;

namespace assets
{
class AssetManager;
class Material;

class Model: public Asset
{
public:
    Model(const std::string& name);
    virtual ~Model();

    struct SceneDeleter
    {
        constexpr SceneDeleter() = default;
        void operator()(const void*) {};
    };

    bool loadFromDisk();
    bool loadFromDisk(AssetManager& assetManager);
    size_t reportSize() const;
    void reload();

    std::unique_ptr<const aiScene, std::function<void(const aiScene*)>> scene;
    std::vector<std::shared_ptr<Material>> materials;

    std::string path;
    std::string basePath;
private:
    Assimp::Importer importer;
};

}

#endif /* MODEL_HPP_ */
