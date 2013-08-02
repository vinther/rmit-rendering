/*
 * GLResourceManager.hpp
 *
 *  Created on: 01/08/2013
 *      Author: svp
 */

#ifndef GLRESOURCEMANAGER_HPP_
#define GLRESOURCEMANAGER_HPP_

#include <memory>

class AssetManager;

class Model;
class Shader;
class Texture;

class Material;

class aiMaterial;

class GLResourceManager
{
public:
    GLResourceManager();
    virtual ~GLResourceManager();

    template <class T>
    T& bufferObject(T& object, AssetManager& assetManager);

private:
};

#endif /* GLRESOURCEMANAGER_HPP_ */
