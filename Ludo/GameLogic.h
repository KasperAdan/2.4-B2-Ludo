#pragma once
#include "BoardLogic.h"
#include <iostream>
#include <vector>
#include <string>
#include "Pawn.h"
#include "Graphics.h"
#include "handDetection.h"

class GameLogic
{
public:
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
