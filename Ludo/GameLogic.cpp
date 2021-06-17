#include "GameLogic.h"
#include <thread>

void graphicsLoop();

GameLogic::GameLogic()
{
}

Graphics graphics;
BoardLogic board;

GameLogic::GameLogic(int amountOfPlayers)
{
	board = BoardLogic(amountOfPlayers);
	graphics = Graphics();
	Dobble d = Dobble(0);

	playerTurn = 0;
	running = true;

	std::thread graphicsThread = std::thread(&graphicsLoop);

	while (running)
	{
		//dobble (dobble detection)
		std::cout << "color " << getStringEnum(board.players[playerTurn].playerColor) << ", its your turn to doble: \n\n";

		int diceFound = -1;
		diceFound = d.findDice();
		std::cout << diceFound <<  "\n\n";
		
		//get choices
		std::vector<int> playerPawns = board.getPawnLocations(board.players[playerTurn]);

		if (playerPawns.size() == 0 && diceFound != 6)
		{
			nextTurn();
			continue;
		}

		std::vector<int> possiblePawns;

		if (diceFound == 6)
		{
			if (board.spawnPawnCheck(&board.players[playerTurn]))
			{
				possiblePawns.push_back(99);
			}
		}

		for (int i : playerPawns)
		{
			if (board.movePawnCheck(i, diceFound))
			{
				possiblePawns.push_back(i);
			}

		}

		std::cout << "the pawns you can move are on position \n";
		int j = 0;
		for (int i : possiblePawns)
		{
			std::cout << "option " << j << ": " << i << "\n";
			j++;
		}

		if (possiblePawns.size() == 0 && diceFound != 6)
		{
			nextTurn();
			continue;
		}

		//choose option (finger detection)
		std::cout << "color " << getStringEnum(board.players[playerTurn].playerColor) << ", enter your selected option: ";
		int pawnValue = -1;
		while (pawnValue >= possiblePawns.size() || pawnValue < 0)
		{
			std::cin >> pawnValue;
		}

		//change board
		if (possiblePawns[pawnValue] == 99)
		{
			//add new pawn on board
			board.spawnPawn(&board.players[playerTurn], graphics);
		}
		else
		{
			//for (int i = 0; i < dobbleValue; i++) {
				board.movePawn(possiblePawns[pawnValue], diceFound, graphics);
				//graphics.movePawn(0, glm::vec3(-4, 0, 4));
				//while (graphics.isMoving(possiblePawns[pawnValue])) {}
			//}
		}

		//next player
		if(diceFound != 6)
			nextTurn();
	}
	
}

GameLogic::~GameLogic()
{
}

void graphicsLoop() {
	graphics.mainLoop();
}

void GameLogic::update()
{
	if (running)
	{
		board.printBoard();

		//dobble (dobble detection)
		std::cout << "color " << getStringEnum(board.players[playerTurn].playerColor) << ", enter your dobble value: ";
		int dobbleValue = -1;

		while (dobbleValue > 6 || dobbleValue <= 0)
		{
			std::cin >> dobbleValue;
		}

		//get choices
		std::vector<int> playerPawns = board.getPawnLocations(board.players[playerTurn]);

		if (playerPawns.size() == 0 && dobbleValue != 6)
		{
			nextTurn();
			return;
		}

		std::vector<int> possiblePawns;

		if (dobbleValue == 6)
		{
			if (board.spawnPawnCheck(&board.players[playerTurn]))
			{
				possiblePawns.push_back(99);
			}
		}

		for (int i : playerPawns)
		{
			if (board.movePawnCheck(i, dobbleValue))
			{
				possiblePawns.push_back(i);
			}

		}

		std::cout << "the pawns you can move are on position \n";
		int j = 0;
		for (int i : possiblePawns)
		{
			std::cout << "option " << j << ": " << i << "\n";
			j++;
		}

		if (possiblePawns.size() == 0)
		{
			nextTurn();
			return;
		}

		//choose option (finger detection)
		std::cout << "color " << getStringEnum(board.players[playerTurn].playerColor) << ", enter your selected option: ";
		int pawnValue = -1;
		while (pawnValue >= possiblePawns.size() || pawnValue < 0)
		{
			std::cin >> pawnValue;
		}

		//change board
		if (possiblePawns[pawnValue] == 99)
		{
			//add new pawn on board
			board.spawnPawn(&board.players[playerTurn], graphics);
		}
		else
		{
			board.movePawn(possiblePawns[pawnValue], dobbleValue, graphics);
			//graphics.movePawn(0, glm::vec3(81, 0, 410));
		}

		//next player
		nextTurn();
	}
}

void GameLogic::nextTurn()
{
	playerTurn++;
	if (playerTurn >= board.players.size())
	{
		playerTurn = 0;
	}
}

std::string GameLogic::getStringEnum(state color)
{
	switch (color)
	{
		case empty:
			return "empty";
			break;
		case blue:
			return "blue";
			break;
		case red:
			return "red";
			break;
		case yellow:
			return "yellow";
			break;
		case green:
			return "green";
			break;
		default:
			break;
	}
}
