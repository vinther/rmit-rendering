/*
 * Material.hpp
 *
 *  Created on: 04/08/2013
 *      Author: svp
 */

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "assets/Asset.hpp"

#include <memory>

#include <glm/glm.hpp>

class aiMaterial;

namespace assets
{

class AssetManager;
class Texture;

class Material: public Asset
{
public:
    Material(const std::string& name);
    virtual ~Material();

    struct MaterialInfo
    {
        glm::vec4 emission;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
    } materialInfo;

    bool translucent;

    std::shared_ptr<assets::Texture> texAmbient;
    std::shared_ptr<assets::Texture> texDiffuse;
    std::shared_ptr<assets::Texture> texSpecular;
    std::shared_ptr<assets::Texture> texNormal;
    std::shared_ptr<assets::Texture> texBump;

    bool loadFromDisk(const std::string& basePath, const aiMaterial& material, AssetManager& assetManager);

    size_t reportSize() const;
    void reload();
};

} /* namespace assets */
#endif /* MATERIAL_HPP_ */
