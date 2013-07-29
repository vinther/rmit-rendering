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

class Camera
{
public:
	Camera();

	glm::vec3 position;
	glm::quat rotation;
};

#endif /* CAMERA_HPP_ */
