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

    const glm::quat& local_rotation() const
    {
        return local_rotation_;
    }

    void local_rotation(const glm::quat& localRotation)
    {
        local_rotation_ = localRotation;
    }

    const glm::vec3& position() const
    {
        return position_;
    }

    void position(const glm::vec3& position)
    {
        position_ = position;
    }

private:
	glm::vec3 position_;
	glm::quat local_rotation_;
};

#endif /* CAMERA_HPP_ */
