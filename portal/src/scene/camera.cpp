/*
 * Camera.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "scene/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

scene::camera::camera()
    : position(glm::vec3(0.0f))
    , field_of_view(75.0f), aspect_ratio(800.0f / 600.0f)
    , near_plane(1.0f), far_plane(4000.0f)
    , horizontal_angle(0.0f), vertical_angle(0.0f)
{
}

glm::mat4 scene::camera::viewProjection() const
{
    return projection() * view();
}

glm::mat4 scene::camera::projection() const {
    return glm::perspective(field_of_view, aspect_ratio, near_plane, far_plane);
}

glm::mat4 scene::camera::view() const {
    return orientation() * glm::translate(glm::mat4(), -position);
}

glm::mat4 scene::camera::orientation() const
{
    glm::mat4 orientation(1.0f);
    orientation = glm::rotate(orientation, vertical_angle, glm::vec3(1.0, 0.0f, 0.0));
    orientation = glm::rotate(orientation, horizontal_angle, glm::vec3(0.0, 1.0f, 0.0));
    return orientation;
}

glm::vec3 scene::camera::forward() const {
    return glm::normalize(glm::vec3(glm::inverse(orientation()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)));
}

glm::vec3 scene::camera::right() const {
    return glm::normalize(glm::vec3(glm::inverse(orientation()) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
}

glm::vec3 scene::camera::up() const {
    return glm::normalize(glm::vec3(glm::inverse(orientation()) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
}

void scene::camera::rotate(float horizontalAngle, float vertialAngle)
{
    this->horizontal_angle += horizontalAngle;
    this->vertical_angle += vertialAngle;
}
