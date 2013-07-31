/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "assets/Material.hpp"

Material::Material(const std::string& name)
    : Asset(name, Asset::Type::TYPE_MATERIAL)
{
    // TODO Auto-generated constructor stub

}

Material::~Material()
{
    // TODO Auto-generated destructor stub
}

bool Material::loadFromDisk(const std::string& name)
{
    return false;
}

size_t Material::reportSize() const
{
    return 0;
}
