#include "Pawn.h"
#include <iostream>

Pawn::Pawn(ObjModel* model, glm::vec4 col, glm::vec3 pos)
{
	objModel = model;
	color = col;
	position = pos;
	moveToTarget = false;
	attacking = false;
	moveTarget = position;
	moveSpeed = 1.25f;
	attackHeight = 1.7f;
	scale = glm::vec3(0.2f);
	atBase = true;
}

Pawn::~Pawn()
{
}

void Pawn::draw()
{
	Drawable::draw();
	objModel->setColor(color);
	objModel->draw();
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

			// If we are attacking move again
			if (attacking)
			{
				moveSpeed = 5.0f;
				attacking = false;
				moveTo(attackTarget);
			}
			// Else reset the move speed
			else
				moveSpeed = 1.25f;
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

bool Pawn::hasAttacked()
{
	return !attacking && !moveToTarget;
}

void Pawn::attack(glm::vec3 target)
{
	attacking = true;
	attackTarget = target;

	// Go above the attacked pawn
	target.y += attackHeight;
	moveTo(target);
}

void Pawn::returnToBase(glm::vec3 basePosition)
{
	moveSpeed = 8.0f;
	moveTo(basePosition);
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
