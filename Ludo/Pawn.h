#pragma once

#include "Drawable.h"
#include "ObjModel.h"

class Pawn : public Drawable
{
private:
	ObjModel* objModel;
	glm::vec4 color;

public:
	Pawn(ObjModel*, glm::vec4 = glm::vec4(1,1,1,1));
	~Pawn();

	virtual void draw() override;
};

