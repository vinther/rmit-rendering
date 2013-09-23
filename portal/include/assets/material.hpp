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
#include <array>

#include <glm/glm.hpp>

class aiMaterial;

namespace assets
{

class data_store;
class image;

class Material: public asset
{
public:
    Material(const std::string& name);
    virtual ~Material();

	std::array<std::shared_ptr<assets::image>, 5> textures;

	glm::vec4 emission;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;

	bool translucent;

    bool loadFromDisk(const std::string& basePath, const aiMaterial& material, data_store& assetManager);

    size_t reportSize() const;
    void reload();
};

} /* namespace assets */
#endif /* MATERIAL_HPP_ */
