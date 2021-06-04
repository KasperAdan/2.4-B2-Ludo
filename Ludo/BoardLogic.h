#pragma once
#include "state.h"
#include "PlayerLogic.h"
#include <vector>

class BoardLogic
{
public:
	state board[40];
	std::vector<PlayerLogic> players;

	BoardLogic(); 
	BoardLogic(int);
	~BoardLogic();
	


private:
	std::vector<int> getPawnLocations(state);
	bool movePawnCheck(int, int);
	void movePawn(int, int);
	PlayerLogic getPlayerByColor(state);
};