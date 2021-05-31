#pragma once

#include <vector>
#include "tigl.h"
#include <glm/glm.hpp>

using tigl::Vertex;

class Drawable
{
	std::vector<Vertex> vertices;

public:
	Drawable();
	~Drawable();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1, 1, 1);

	virtual void init();
	virtual void update(float deltaTime);
	virtual void draw();
};

