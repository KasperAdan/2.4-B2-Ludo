#pragma once

#include "Drawable.h"

class Texture;

class Board : public Drawable
{
private:
	Texture* texture;

public:
	Board(Texture*);
	~Board();

	virtual void init() override;
	virtual void draw() override;
};

