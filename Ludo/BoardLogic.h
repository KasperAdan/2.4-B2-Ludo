#pragma once
#include "state.h"
#include "PlayerLogic.h"
#include <vector>
#include <string>
#include "Graphics.h"

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
	void spawnPawn(PlayerLogic *, Graphics);
	bool spawnPawnCheck(PlayerLogic*);
	bool movePawnCheck(int, int);
	void movePawn(int, int, Graphics);
	PlayerLogic getPlayerByColor(state);
	PlayerLogic* getPlayerByColorPointer(state);
	void printBoard();

private:
	std::string getStringEnum(state);
};