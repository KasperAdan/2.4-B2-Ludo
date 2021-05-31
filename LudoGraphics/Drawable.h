#pragma once

#include <vector>
#include "tigl.h"
#include <glm/glm.hpp>
#include "Texture.h"

using tigl::Vertex;

class Drawable
{
protected:
	std::vector<Vertex> vertices;
	Texture* texture;

public:
	Drawable();
	Drawable(Texture*);
	~Drawable();

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);

	virtual void init();
	virtual void update(float deltaTime);
	virtual void draw();
};

