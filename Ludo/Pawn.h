#pragma once

#include "Drawable.h"
#include "ObjModel.h"
#include "state.h"

class Graphics;

class Pawn : public Drawable
{
private:
	ObjModel* objModel;
	glm::vec4 color;

	glm::vec3 moveTarget;
	bool moveToTarget;
	float moveSpeed;

	glm::vec3 attackTarget;
	bool attacking;
	float attackHeight;

	glm::vec3 directionToTarget(glm::vec3);
	float distanceToTarget(glm::vec3);

	Graphics* gph;
	state enemyColor;
	int enemyIndex;

public:
	Pawn(Graphics*, ObjModel*, glm::vec4 = glm::vec4(1,1,1,1), glm::vec3 = glm::vec3(0,0,0));
	~Pawn();

	virtual void update(float deltaTime) override;
	virtual void draw() override;
	void moveTo(glm::vec3);
	void attack(glm::vec3, int, state);
	void returnToBase(glm::vec3);

	bool reachedTarget();
	bool hasAttacked();

	bool atBase;
};

