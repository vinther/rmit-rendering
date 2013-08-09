/*
 * Camera.cpp
 *
 *  Created on: 27/07/2013
 *      Author: svp
 */

#include "scene/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

scene::Camera::Camera()
    : position(glm::vec3(0.0f))
    , fieldOfView(75.0f), aspectRatio(800.0f / 600.0f)
    , nearPlane(1.0f), farPlane(10000.0f)
    , horizontalAngle(0.0f), verticalAngle(0.0f)
{
}

glm::mat4 scene::Camera::getViewProjectionMatrix() const
{
    return projection() * view();
}

glm::mat4 scene::Camera::projection() const {
    return glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
}

glm::mat4 scene::Camera::view() const {
    return orientation() * glm::translate(glm::mat4(), -position);
}

glm::mat4 scene::Camera::orientation() const
{
    glm::mat4 orientation(1.0f);
    orientation = glm::rotate(orientation, verticalAngle, glm::vec3(1.0, 0.0f, 0.0));
    orientation = glm::rotate(orientation, horizontalAngle, glm::vec3(0.0, 1.0f, 0.0));
    return orientation;
}

glm::vec3 scene::Camera::forward() const {
    return glm::vec3(glm::inverse(orientation()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
}

glm::vec3 scene::Camera::right() const {
    return glm::vec3(glm::inverse(orientation()) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

glm::vec3 scene::Camera::up() const {
    return glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

void scene::Camera::rotate(float horizontalAngle, float vertialAngle)
{
    this->horizontalAngle += horizontalAngle;
    this->verticalAngle += vertialAngle;
}
