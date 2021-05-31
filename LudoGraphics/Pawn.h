#pragma once
#include "Drawable.h"
class Pawn : public Drawable
{
public:
	Pawn();
	Pawn(Texture*);
	~Pawn();

	virtual void init() override;
};

