#pragma once

#include "Drawable.h"

class Board : public Drawable
{
public:
	Board(Texture*);
	~Board();

	virtual void init() override;
};

