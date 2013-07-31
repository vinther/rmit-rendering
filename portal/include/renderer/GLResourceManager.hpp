/*
 * GLResourceManager.hpp
 *
 *  Created on: 01/08/2013
 *      Author: svp
 */

#ifndef GLRESOURCEMANAGER_HPP_
#define GLRESOURCEMANAGER_HPP_

class Model;
class Shader;
class Texture;

class GLResourceManager
{
public:
    GLResourceManager();
    virtual ~GLResourceManager();

    template <class T>
    T& bufferObject(T& object);
};

#endif /* GLRESOURCEMANAGER_HPP_ */
