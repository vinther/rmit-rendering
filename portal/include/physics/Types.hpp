/*
 * Types.hpp
 *
 *  Created on: 06/08/2013
 *      Author: svp
 */

#ifndef PHYSICS_TYPES_HPP_
#define PHYSICS_TYPES_HPP_

#include <array>

#include <glm/glm.hpp>

namespace physics
{

struct Triangle
{
    std::array<glm::vec3, 3> vertices;
};

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct AABB
{
    glm::vec3 min, max;
};

}

#endif /* PHYSICS_TYPES_HPP_ */
