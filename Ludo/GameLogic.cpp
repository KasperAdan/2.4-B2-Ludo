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
	
	HandDetection h = HandDetection(1);

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
		//std::cin >> diceFound;
		
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
		int j = 1;
		for (int i : possiblePawns)
		{
			if (i == 99)
			{
				std::cout << "\t option " << j << ": " << "New Pawn" << "\n";
				j++;
				continue;
			}
			std::cout << "\t option " << j << ": " << i << "\n";
			j++;
		}

		if (possiblePawns.size() == 0 && diceFound != 6)
		{
			nextTurn();
			continue;
		}

		int pawnValue = -1;
		if (possiblePawns.size() < 2) {
			pawnValue = 0;
		}
		else {
			//choose option (finger detection)
			std::cout << "color " << getStringEnum(board.players[playerTurn].playerColor) << ", enter your selected option: ";

			// Display options in 3D scene
			graphics.drawNumbers(board.players[playerTurn].playerColor,
				possiblePawns.size() > 0 ? possiblePawns.at(0) : -1,
				possiblePawns.size() > 1 ? possiblePawns.at(1) : -1,
				possiblePawns.size() > 2 ? possiblePawns.at(2) : -1,
				possiblePawns.size() > 3 ? possiblePawns.at(3) : -1);

			while (pawnValue >= possiblePawns.size() || pawnValue < 0)
			{
                pawnValue = h.findFingers() - 1;
				// std::cin >> pawnValue;
			}

			graphics.stopDrawingNumbers();
		}

		//change board
		if (possiblePawns[pawnValue] == 99)
		{
			//add new pawn on board
			board.spawnPawn(&board.players[playerTurn], graphics);
		}
		else
		{
			board.movePawn(possiblePawns[pawnValue], diceFound, graphics);
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
		case state::empty:
			return "empty";
			break;
		case state::blue:
			return "blue";
			break;
		case state::red:
			return "red";
			break;
		case state::yellow:
			return "yellow";
			break;
		case state::green:
			return "green";
			break;
		default:
			break;
	}
}
