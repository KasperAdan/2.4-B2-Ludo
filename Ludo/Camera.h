#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

class Camera
{
protected:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);

public:
	Camera();
	~Camera();

	virtual glm::mat4 getViewMatrix();
	virtual void update(GLFWwindow*);

	void moveTo(glm::vec3 pos, glm::vec3 rot);
};

