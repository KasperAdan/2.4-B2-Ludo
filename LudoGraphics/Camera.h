#pragma once

#include <glm/glm.hpp>

class Camera
{
private:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);

public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();

	void moveTo(glm::vec3 pos, glm::vec3 rot);
};

