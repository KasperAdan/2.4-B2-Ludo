#pragma once

#include "Drawable.h"
#include "ObjModel.h"

class Pawn : public Drawable
{
private:
	ObjModel* objModel;
	glm::vec4 color;

	glm::vec3 moveTarget;
	bool moveToTarget;
	float moveSpeed;

	bool attacking;
	float attackHeight;

	glm::vec3 directionToTarget(glm::vec3);
	float distanceToTarget(glm::vec3);

public:
	Pawn(ObjModel*, glm::vec4 = glm::vec4(1,1,1,1), glm::vec3 = glm::vec3(0,0,0));
	~Pawn();

	virtual void update(float deltaTime) override;
	virtual void draw() override;
	void moveTo(glm::vec3);
	void attackTarget(glm::vec3);

	bool reachedTarget();
	bool hasAttacked();
};

