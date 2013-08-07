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

	struct State
	{
	    glm::vec3 position;
        glm::quat rotation;
	} state;
};

}

#endif /* CAMERA_HPP_ */
