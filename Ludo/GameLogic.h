#pragma once
#include "BoardLogic.h"
#include <iostream>
#include <vector>
#include <string>
#include "Pawn.h"
#include "Graphics.h"

class GameLogic
{
public:
	//BoardLogic board;
	//Graphics graphics;
	int playerTurn;
	bool running;

	GameLogic();
	GameLogic(int);
	~GameLogic();

	void nextTurn();
	void update();

private:
	std::string getStringEnum(state);
};
