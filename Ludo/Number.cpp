#include "Number.h"

Number::Number(Texture* texture)
{
	tex = texture;
}

Number::~Number()
{
}

void Number::init()
{
	vertices.push_back(Vertex::PT(glm::vec3(0, -1, 0), glm::vec2(0, 0)));
	vertices.push_back(Vertex::PT(glm::vec3(0, 0, 0), glm::vec2(0, 1)));
	vertices.push_back(Vertex::PT(glm::vec3(1, 0, 0), glm::vec2(1, 1)));
	vertices.push_back(Vertex::PT(glm::vec3(1, -1, 0), glm::vec2(1, 0)));
}

void Number::draw()
{
	tex->bind();
	tigl::shader->setColorMult(glm::vec4(1, 1, 1, 1));
	Drawable::draw();
	tigl::drawVertices(GL_QUADS, vertices);
}
