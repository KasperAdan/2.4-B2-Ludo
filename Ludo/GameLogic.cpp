#include "GameLogic.h"

GameLogic::GameLogic()
{
}

GameLogic::GameLogic(int amountOfPlayers)
{
	board = BoardLogic(amountOfPlayers);


	running = true;
	while (running)
	{
		//dobble (dobble detection)
		int dobbleValue;
		std::cin >> dobbleValue;

		//get choices
		std::vector<int> playerPawns = board.getPawnLocations(board.players[playerTurn]);


		//choose option (finger detection)

		//change board

		//next player
		nextTurn();
	}
}

GameLogic::~GameLogic()
{
}

void GameLogic::nextTurn()
{
	playerTurn++;
	if (playerTurn >= board.players.size())
	{
		playerTurn = 0;
	}
}
