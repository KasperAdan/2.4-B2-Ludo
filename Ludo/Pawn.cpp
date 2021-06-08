#include "Pawn.h"

Pawn::Pawn()
{
	texture = nullptr;
	moveToTarget = false;
	attacking = false;
	moveTarget = position;
	moveSpeed = 2.0f;
	attackHeight = 0.8f;
}

Pawn::Pawn(glm::vec3 pos)
{
	position = pos;
	texture = nullptr;
	moveToTarget = false;
	attacking = false;
	moveTarget = position;
	moveSpeed = 2.0f;
	attackHeight = 1.5f;
}

Pawn::Pawn(Texture* tex)
{
	moveTarget = position;
	moveToTarget = false;
	attacking = false;
	texture = tex;
	moveSpeed = 2.0f;
	attackHeight = 1.5f;
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

void Pawn::update(float deltaTime)
{
	if (moveToTarget) {
		if (distanceToTarget(moveTarget) > 0.05f) {
			// Move to target
			position += directionToTarget(moveTarget) * deltaTime * moveSpeed;
		}
		else
		{
			// We reached our target
			moveToTarget = false;
			position = moveTarget;
		}
	}
	else if (attacking)
	{
		if (distanceToTarget(glm::vec3(position.x, position.y - attackHeight, position.z)) > 0.05f) {
			position += directionToTarget(glm::vec3(position.x, position.y - attackHeight, position.z)) * deltaTime * moveSpeed;
		}
		else {
			attacking = false;
			position = glm::vec3(position.x, position.y - attackHeight, position.z);
			moveTarget = position;
		}
	}
}

void Pawn::moveTo(glm::vec3 target)
{
	moveToTarget = true;
	moveTarget = target;
}

bool Pawn::reachedTarget()
{
	return !moveToTarget;
}

void Pawn::attackTarget(glm::vec3 target)
{
	// Go above the standing pawn
	target.y += attackHeight;
	moveTarget = target;

	moveToTarget = true;
	attacking = true;
}

glm::vec3 Pawn::directionToTarget(glm::vec3 target)
{
	// Calculate difference between position and target
	glm::vec3 diff = glm::vec3(
		target.x - position.x,
		target.y - position.y,
		target.z - position.z
	);

	// Normalize result and return
	return glm::normalize(diff);
}

float Pawn::distanceToTarget(glm::vec3 target)
{
	// Calculate difference between position and target 
	glm::vec3 diff = glm::vec3(
		position.x - target.x,
		position.y - target.y,
		position.z - target.z
	);

	// Calculate the magnitude of the difference vector
	float distance = glm::sqrt(
		glm::pow(diff.x, 2.0f) + 
		glm::pow(diff.y, 2.0f) + 
		glm::pow(diff.z, 2.0f)
	);

	return distance;
}
