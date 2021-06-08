#pragma once
#include "BoardLogic.h"
#include <iostream>
#include <vector>
#include <string>


class GameLogic
{
public:
	BoardLogic board;
	int playerTurn;
	bool running;

	GameLogic();
	GameLogic(int);
	~GameLogic();

	void nextTurn();

	

private:
	std::string getStringEnum(state);
};
