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

    const glm::vec3& getPosition() const
    {
        return position;
    }

    void setPosition(const glm::vec3& position)
    {
        this->position = position;
    }

    const glm::quat& getRotation() const
    {
        return rotation;
    }

    void setRotation(const glm::quat& rotation)
    {
        this->rotation = rotation;
    }
private:
	glm::vec3 position;

	glm::quat rotation;
};

#endif /* CAMERA_HPP_ */
