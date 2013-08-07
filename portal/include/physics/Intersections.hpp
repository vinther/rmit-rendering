/*
 * Intersection.hpp
 *
 *  Created on: 06/08/2013
 *      Author: svp
 */

#ifndef INTERSECTIONS_HPP_
#define INTERSECTIONS_HPP_

#include <glm/glm.hpp>

#include "physics/Types.hpp"

namespace physics
{

inline bool rayAABBIntersection(
        const glm::vec3& position,
        const glm::vec3& direction,
        const glm::vec3& aabbMin,
        const glm::vec3& aabbMax,
        float& distance)
{
    float tMin = 0.0f;
    float tMax = std::numeric_limits<float>::max();

    glm::vec3 OBBposition_worldspace(1.0f, 0.0f, 0.0f);

    glm::vec3 delta = OBBposition_worldspace - position;

    glm::vec3 xaxis(1.0f, 0.0f, 0.0f);
    float e = glm::dot(xaxis, delta);
    float f = glm::dot(direction, xaxis);

    // Beware, don't do the division if f is near 0 ! See full source code for details.
    float t1 = (e+aabbMin.x) / f; // Intersection with the "left" plane
    float t2 = (e+aabbMax.x) / f; // Intersection with the "right" plane

    if (t1>t2){ // if wrong order
        float w=t1;t1=t2;t2=w; // swap t1 and t2
    }

    // tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
    if ( t2 < tMax ) tMax = t2;
    // tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
    if ( t1 > tMin ) tMin = t1;

    if (tMax < tMin )
        return false;

    distance = tMin;

    return true;
}

inline bool pointAABBIntersection(const glm::vec3& point, const AABB& aabb)
{
    return (point.x < aabb.max.x && point.y < aabb.max.y && point.z < aabb.max.z &&
            point.x > aabb.min.x && point.y > aabb.min.y && point.z > aabb.min.z);
}

inline bool triangleAABBIntersection(const Triangle& tri, const AABB& aabb)
{
    return (pointAABBIntersection(tri.vertices[0], aabb) ||
            pointAABBIntersection(tri.vertices[1], aabb) ||
            pointAABBIntersection(tri.vertices[2], aabb));
}

}

#endif /* INTERSECTIONS_HPP_ */
