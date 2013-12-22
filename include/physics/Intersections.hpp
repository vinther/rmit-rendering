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

#include "Utilities.hpp"

namespace physics
{

extern "C"
{
    int triBoxOverlap(float boxcenter[3], float boxhalfsize[3], float triverts[3][3]);
}

inline bool rayAABBIntersection(
        const glm::vec3& origin,
        const glm::vec3& direction,
        const glm::vec3& aabbMin,
        const glm::vec3& aabbMax,
        float& distance)
{
    using glm::max;
    using glm::min;

    // r.dir is unit direction vector of ray
    glm::vec3 dirfrac = 1.0f / direction;

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (aabbMin.x - origin.x) * dirfrac.x;
    float t2 = (aabbMax.x - origin.x) * dirfrac.x;
    float t3 = (aabbMin.y - origin.y) * dirfrac.y;
    float t4 = (aabbMax.y - origin.y) * dirfrac.y;
    float t5 = (aabbMin.z - origin.z) * dirfrac.z;
    float t6 = (aabbMax.z - origin.z) * dirfrac.z;

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behind us
    if (tmax < 0)
    {
        distance = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        distance = tmax;
        return false;
    }

    distance = tmin;
    return true;
}

const float inf = std::numeric_limits<float>::infinity();

inline bool pointAABBIntersection(const glm::vec3& point, const AABB& aabb)
{
    return (point.x < aabb.max.x && point.y < aabb.max.y && point.z < aabb.max.z &&
            point.x > aabb.min.x && point.y > aabb.min.y && point.z > aabb.min.z);
}

inline bool triangleAABBIntersection(const Triangle& tri, const AABB& aabb)
{
    if (pointAABBIntersection(tri.vertices[0], aabb) ||
        pointAABBIntersection(tri.vertices[1], aabb) ||
        pointAABBIntersection(tri.vertices[2], aabb))
    {
        return true;
    } else
    {
        Triangle triCopy = tri;
        glm::vec3 boxhalfsize = (aabb.max - aabb.min) * 0.5f;
        glm::vec3 boxcenter = aabb.min + boxhalfsize;

        return 1 == triBoxOverlap((float*) &boxcenter, (float*) &boxhalfsize, (float (*)[3]) &triCopy);
    }
}



}

#endif /* INTERSECTIONS_HPP_ */
