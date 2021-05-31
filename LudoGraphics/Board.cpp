#include "Board.h"

Board::Board() 
{
}

Board::~Board()
{
}

void Board::init()
{
	vertices.push_back(Vertex::P(glm::vec3(-1, 1, 0)));
	vertices.push_back(Vertex::P(glm::vec3(-1, -1, 0)));
	vertices.push_back(Vertex::P(glm::vec3(1, -1, 0)));
	vertices.push_back(Vertex::P(glm::vec3(1, 1, 0)));
}

