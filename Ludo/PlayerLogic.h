#pragma once
#include "state.h"

class PlayerLogic
{
public:
	state playerColor;
	int boardOffset;
	state finish[4] = { empty, empty, empty, empty };
	int homePawns = 4;


	PlayerLogic();
	PlayerLogic(state, int);
	~PlayerLogic();

	void addHomePawn();
	void removeHomePawn();

private:

};

