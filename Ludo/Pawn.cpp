#include "Pawn.h"

Pawn::Pawn()
{
	texture = nullptr;
}

Pawn::Pawn(Texture* tex)
{
	texture = tex;
}

Pawn::~Pawn()
{
}

void Pawn::init()
{
	vertices.push_back(Vertex::P(glm::vec3(-1, 1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(-1, -1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(1, -1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(1, 1, 1)));
	
	vertices.push_back(Vertex::P(glm::vec3(-1, 1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(-1, -1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(1, -1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(1, 1, -1)));

	vertices.push_back(Vertex::P(glm::vec3(1, 1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(1, -1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(1, -1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(1, 1, 1)));

	vertices.push_back(Vertex::P(glm::vec3(-1, 1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(-1, -1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(-1, -1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(-1, 1, 1)));

	vertices.push_back(Vertex::P(glm::vec3(-1, 1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(-1, 1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(1, 1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(1, 1, -1)));

	vertices.push_back(Vertex::P(glm::vec3(-1, -1, -1)));
	vertices.push_back(Vertex::P(glm::vec3(-1, -1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(1, -1, 1)));
	vertices.push_back(Vertex::P(glm::vec3(1, -1, -1)));
}
