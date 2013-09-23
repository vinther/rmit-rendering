#ifndef SCENE_CAMERA_HPP_
#define SCENE_CAMERA_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace scene
{

class camera
{
public:
	camera();

	glm::vec3 position;

	float field_of_view;
	float aspect_ratio;
	float near_plane;
	float far_plane;

	float horizontal_angle, vertical_angle;

    glm::mat4 viewProjection() const;
    glm::mat4 projection() const;
    glm::mat4 view() const;

    glm::mat4 orientation() const;

    glm::vec3 forward() const;
    glm::vec3 right() const;
    glm::vec3 up() const;

    void rotate(float horizontalAngle, float vertialAngle);
};

}

#endif /* SCENE_CAMERA_HPP_ */
