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
	
	std::vector<int> getPawnLocations(state);
	std::vector<int> getPawnLocations(PlayerLogic);
	void spawnPawn(state);
	bool movePawnCheck(int, int);
	void movePawn(int, int);
	PlayerLogic getPlayerByColor(state);

private:
	
};