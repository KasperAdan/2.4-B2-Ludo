#pragma once
#include "state.h"

class PlayerLogic
{
public:
	state playerColor;
	int boardOffset;
	state finish[4] = { state::empty, state::empty, state::empty, state::empty };
	int homePawns;


	PlayerLogic();
	PlayerLogic(state, int);
	~PlayerLogic();

	void addHomePawn();
	void removeHomePawn();
};

