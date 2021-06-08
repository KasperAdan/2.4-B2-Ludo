#include "Pawn.h"

Pawn::Pawn(ObjModel* model, glm::vec4 col)
{
	objModel = model;
	color = col;
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
