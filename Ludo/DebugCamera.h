#pragma once

#include "Camera.h"

class DebugCamera : public Camera
{
public:
	DebugCamera(GLFWwindow*);
	~DebugCamera();

	virtual glm::mat4 getViewMatrix() override;
	virtual void update(GLFWwindow*) override;

private:
	void move(float angle, float fac);
};

