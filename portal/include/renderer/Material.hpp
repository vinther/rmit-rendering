/*
 * Material.hpp
 *
 *  Created on: 31/07/2013
 *      Author: svp
 */

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "assets/Asset.hpp"

class AssetManager;
class Shader;
class Texture;
class Model;

class aiMaterial;

typedef unsigned int GLuint;

class Material
{
public:
    Material();
    virtual ~Material();

    struct MaterialInfo
    {
        glm::vec4 Ka;
        glm::vec4 Kd;
        glm::vec4 Ks;
    } materialInfo;

    std::weak_ptr<Texture> texAmbient;
    std::weak_ptr<Texture> texDiffuse;
    std::weak_ptr<Texture> texSpecular;
    std::weak_ptr<Texture> texNormal;
    std::weak_ptr<Texture> texBump;

    std::weak_ptr<Shader> shader;

    std::string name;
    GLuint texIDTEST;

    void activate();
    void createFromAssimpMaterial(const aiMaterial& material, const Model& model, AssetManager& assetManager);
};

#endif /* MATERIAL_HPP_ */
