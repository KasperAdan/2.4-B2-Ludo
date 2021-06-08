#pragma once
#include "Drawable.h"
class Pawn : public Drawable
{
private:
	glm::vec3 moveTarget;
	bool moveToTarget;
	float moveSpeed;

	bool attacking;
	float attackHeight;

	glm::vec3 directionToTarget(glm::vec3);
	float distanceToTarget(glm::vec3);

public:
	Pawn();
	Pawn(glm::vec3);
	Pawn(Texture*);
	~Pawn();

	virtual void init() override;
	virtual void update(float deltaTime) override;
	void moveTo(glm::vec3);
	bool reachedTarget();
	void attackTarget(glm::vec3);
};

