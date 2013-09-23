/*
 * Material.cpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#include "renderer/resources/Material.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

#include "renderer/ResourceManager.hpp"
#include "renderer/resources/Texture.hpp"

#include "assets/Texture.hpp"
#include "assets/DiskFile.hpp"

#include "Utilities.hpp"

renderer::resources::Material::Material(std::shared_ptr<const asset_type> asset)
    : AssetResource(asset)
    , translucent(false)
{
}

renderer::resources::Material::~Material()
{
}

void renderer::resources::Material::activate() const
{
	for (const auto& texturePair : {
			std::make_pair(GL_TEXTURE0, MaterialInfo::TextureBuffers::TEXTURE_EMISSIVE),
			std::make_pair(GL_TEXTURE1, MaterialInfo::TextureBuffers::TEXTURE_AMBIENT),
			std::make_pair(GL_TEXTURE2, MaterialInfo::TextureBuffers::TEXTURE_DIFFUSE),
			std::make_pair(GL_TEXTURE3, MaterialInfo::TextureBuffers::TEXTURE_SPECULAR),
			std::make_pair(GL_TEXTURE4, MaterialInfo::TextureBuffers::TEXTURE_BUMP)})
	{
	    if (materialInfo.buffers[texturePair.second])
	    {
	        glActiveTexture(texturePair.first);
	        glBindTexture(GL_TEXTURE_2D, materialInfo.buffers[texturePair.second]);
	    }
	}
}

void renderer::resources::Material::loadFromAsset(ResourceManager& resourceManager)
{
    const auto& material = *(asset);

    glDeleteTextures(MaterialInfo::TextureBuffers::TEXTURE_LAST_INDEX, materialInfo.buffers.data());
    glGenTextures(MaterialInfo::TextureBuffers::TEXTURE_LAST_INDEX, materialInfo.buffers.data());

    unsigned int typeInt = MaterialInfo::TextureBuffers::TEXTURE_EMISSIVE;
    for (const auto& texture: material.textures)
    {
    	if (texture)
    	{
    		const auto d = resourceManager.getByAsset<Texture>(texture);
    		materialInfo.buffers[typeInt] = d->buffer;

    		assert(d->state == Texture::State::READY);
    		assert(d->buffer != 0);
    	}

    	++typeInt;
    }

    materialInfo.emission = material.emission;
    materialInfo.ambient = material.ambient;
    materialInfo.diffuse = material.diffuse;
    materialInfo.specular = material.specular;
    materialInfo.shininess = material.shininess;

    translucent = material.translucent;
}

renderer::resources::Material::MaterialInfo::MaterialInfo()
    : buffers({{0, 0, 0, 0, 0}})
    , shininess(0.0f)
{
}
