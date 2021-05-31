#pragma once

#include "Drawable.h"

class Board : public Drawable
{
public:
	Board();
	~Board();

	virtual void init() override;
};

