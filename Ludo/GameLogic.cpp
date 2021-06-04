#include "GameLogic.h"

GameLogic::GameLogic()
{
}

GameLogic::GameLogic(int amountOfPlayers)
{
	for (int i = 0; i < amountOfPlayers; i++)
	{
		players.push_back(PlayerLogic((state)(1 + i), (10 * i)));
	}
}

GameLogic::~GameLogic()
{
}
