#include "BoardLogic.h"
#include <iostream>

BoardLogic::BoardLogic()
{
}


BoardLogic::BoardLogic(int amountOfPlayers)
{
	for (int i = 0; i < amountOfPlayers; i++)
	{
		PlayerLogic player = PlayerLogic((state)(1 + i), (10 * i));
		players.push_back(player);
	}

	for (int i = 0; i < 40; i++)
	{
		board[i] = state::empty;
	}
}

BoardLogic::~BoardLogic()
{
}

//returns all the locations of the pawns of a player
std::vector<int> BoardLogic::getPawnLocations(state player)
{
	std::vector<int> locations;
	for (int i = 0; i < 40; i++)
	{
		if (board[i] == player) 
		{
			locations.push_back(i);
		}
	}
	return locations;
}

//returns all the locations of the pawns of a player
std::vector<int> BoardLogic::getPawnLocations(PlayerLogic player)
{
	std::vector<int> locations;
	for (int i = 0; i < 40; i++)
	{
		if (board[i] == player.playerColor)
		{
			locations.push_back(i);
		}
	}
	if (locations.size() == 0)
	{
		return {};
	}

	return locations;
}

void BoardLogic::spawnPawn(PlayerLogic *player, Graphics gph)
{
	if (player->homePawns != 0)
	{
		player->removeHomePawn();
		if (board[player->boardOffset] != state::empty)
		{
			PlayerLogic enemy = getPlayerByColor(board[player->boardOffset]);
			enemy.addHomePawn();
			gph.moveFromBase(player->playerColor, player->boardOffset, true, enemy.playerColor);
		}
		else
			gph.moveFromBase(player->playerColor, player->boardOffset, false, state::empty);
		
		board[player->boardOffset] = player->playerColor;
	}
}

bool BoardLogic::spawnPawnCheck(PlayerLogic* player)
{
	if (board[player->boardOffset] != player->playerColor)
	{
		return true;
	}
	return false;
}

//this method checks if a move is possible
//this can be used to check if the user can choose this pawn to move or not
bool BoardLogic::movePawnCheck(int location, int amount)
{
	PlayerLogic player = getPlayerByColor(board[location]);

	if (board[(location + amount) % 40] == player.playerColor)//check for own pawn in new position
	{
		return false;
	}

	if (player.boardOffset == 0 && location + amount < 40)
	{
		return true;
	}

	if ((location + amount) >= 40 && player.boardOffset != 0)
	{
		if ((location + amount) % 40 < player.boardOffset)// true: the pawn doesnt reach finish
		{
			return true;
		}
	}
	if ((location + amount) < player.boardOffset && player.boardOffset != 0 || location >= player.boardOffset)// true: the pawn doesnt reach finish
	{
		return true;
	}
	else //pawn reaches finish
	{
		int finishLocation;
		if (player.boardOffset == 0)
		{
			finishLocation = (location + amount) - 40;
		}
		else
		{
			finishLocation = (location + amount) - ((player.boardOffset + 40) % 40);
		}

		if (finishLocation > 3)
		{
			finishLocation = 3 - (finishLocation - 3);
		}
		if (player.finish[finishLocation] == state::empty)//check if finish spot is free
		{
			return true;
		}
	}
	return false;//move isnt possible
}

//this method moves the selected pawn
void BoardLogic::movePawn(int location, int amount, Graphics gph)
{
	PlayerLogic* player = getPlayerByColorPointer(board[location]);
	bool reachesFinish = false;

	if (player->boardOffset == 0 && location + amount >= 40)
	{
		reachesFinish = true;
	}

	if ((location + amount) >= 40 && player->boardOffset != 0)
	{
		if ((location + amount) % 40 >= player->boardOffset)
		{
			reachesFinish = true;
		}
	}
	if ((location + amount) >= player->boardOffset && player->boardOffset != 0 && location < player->boardOffset)
	{
		reachesFinish = true;
	}
	if (reachesFinish)
	{
		int finishLocation;
		if (player->boardOffset == 0)
		{
			finishLocation = (location + amount) - 40;
		}
		else
		{
			finishLocation = (location + amount) - ((player->boardOffset + 40) % 40);
		}

		if (finishLocation > 3)
		{
			finishLocation = 3 - (finishLocation - 3);
		}
		player->finish[finishLocation] = player->playerColor;
		board[location] = state::empty;
		gph.finishPawn(player->playerColor, finishLocation, location);
		return;
	}
	if (board[(location + amount) % 40] != state::empty)//check for an enemy on new position
	{
		state enemyColor = board[(location + amount) % 40];
		PlayerLogic* enemy = getPlayerByColorPointer(enemyColor);
		enemy->homePawns++;
		gph.attackPawn(location, (location + amount) % 40, enemy->playerColor);
	}
	else {
		gph.movePawn(location, (location + amount) % 40);
	}
	board[(location + amount) % 40] = player->playerColor;
	board[location] = state::empty;
}

//returns the PlayerLogic object of the input color
PlayerLogic BoardLogic::getPlayerByColor(state color) 
{
	for(PlayerLogic player : players)
	{
		if (player.playerColor == color)
		{
			return player;
		}
	}
	return PlayerLogic();
}

//returns the PlayerLogic object pointer of the input color 
PlayerLogic* BoardLogic::getPlayerByColorPointer(state color)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].playerColor == color)
		{
			return &players[i];
		}
	}
	PlayerLogic player = PlayerLogic(state::empty, 0);
	return &player;
}


void BoardLogic::printBoard() 
{
	std::cout << "home pawns\n";

	for (PlayerLogic player : players)
	{
		std::cout << "\tcolor:" << getStringEnum(player.playerColor) << ":\t";
		for (int i = 0; i < player.homePawns; i++)
		{
			std::cout << " . ";
		}
		std::cout << "\n";
	}

	std::cout << "finish pawns\n";

	for (PlayerLogic player : players)
	{
		std::cout << "\tcolor:" << getStringEnum(player.playerColor) << ":\t";
		for (int i = 0; i < 4; i++)
		{
			if (player.finish[i] == state::empty)
			{
				std::cout << " . ";
			}
			else
			{
				std::cout << " | ";
			}
			
		}
		std::cout << "\n";
	}

	std::cout << "board pawns: \n";

	for (state s : board)
	{
		switch (s)
		{
		case state::empty:
			std::cout << "_";
			break;
		case state::blue:
			std::cout << "B";
			break;
		case state::red:
			std::cout << "R";
			break;
		case state::yellow:
			std::cout << "Y";
			break;
		case state::green:
			std::cout << "G";
			break;
		default:
			break;
		}
	}
	std::cout << "\n";
	std::cout << "\n";
}

std::string BoardLogic::getStringEnum(state color)
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