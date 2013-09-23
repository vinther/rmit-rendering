#ifndef ASSET_SCENE_HPP_
#define ASSET_SCENE_HPP_

#include "assets/asset.hpp"

#include <memory>
#include <array>

#include <SDL2/SDL.h>
#include <assimp/Importer.hpp>

#include "asset_store.hpp"

class aiScene;

namespace assets
{

class material;

class scene: public asset
{
public:
    scene(const std::string& name);
    virtual ~scene();

    struct SceneDeleter
    {
        constexpr SceneDeleter() = default;
        void operator()(const void*) {};
    };

    bool loadFromDisk();
    bool loadFromDisk(asset_store& assetManager);
    size_t reportSize() const;
    void reload();

    std::unique_ptr<const aiScene, std::function<void(const aiScene*)>> assimp_scene;
    std::vector<handle<assets::material> > materials;

    std::string path;
    std::string basePath;
private:
    Assimp::Importer importer;
};

}

#endif /* ASSET_SCENE_HPP_ */
