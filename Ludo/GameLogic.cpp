#include "GameLogic.h"

GameLogic::GameLogic()
{
}

GameLogic::GameLogic(int amountOfPlayers)
{
	board = BoardLogic(amountOfPlayers);
}

GameLogic::~GameLogic()
{
}
