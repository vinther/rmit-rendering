/*
 * Camera.hpp
 *
 *  Created on: 26/07/2013
 *      Author: s3443408
 */

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace scene
{

class Camera
{
public:
	Camera();

	glm::vec3 position;

	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	float horizontalAngle, verticalAngle;

    glm::mat4 getViewProjectionMatrix() const;
    glm::mat4 projection() const;
    glm::mat4 view() const;

    glm::mat4 orientation() const;

    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    void rotate(float horizontalAngle, float vertialAngle);
};

}

#endif /* CAMERA_HPP_ */
