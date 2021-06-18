#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 viewMatrix(1.0f);
	viewMatrix = glm::rotate(viewMatrix, rotation.x, glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.y, glm::vec3(0, 1, 0));
	viewMatrix = glm::rotate(viewMatrix, rotation.z, glm::vec3(0, 0, 1));
	viewMatrix = glm::translate(viewMatrix, position);
	return viewMatrix;
}

void Camera::update(GLFWwindow*)
{
}

void Camera::moveTo(glm::vec3 pos, glm::vec3 rot)
{
	position = pos;
	rotation = glm::vec3(glm::radians(rot.x), glm::radians(rot.y), glm::radians(rot.z));
}
