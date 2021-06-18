#pragma once
#include "Drawable.h"
#include "Texture.h"

class Number : public Drawable
{
private:
	Texture* tex;

public:
	Number(Texture* texture);
	~Number();

	virtual void init() override;
	virtual void draw() override;
};

