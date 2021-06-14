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
			gph.moveFromBase(player->playerColor, player->boardOffset, true);
		}
		else
			gph.moveFromBase(player->playerColor, player->boardOffset, false);
		
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
	
	if (location + amount < player.boardOffset) 
	{
		int offsetLocation = location + amount + (40 - player.boardOffset);
		if (offsetLocation < 40)//check if pawn reaches finish
		{
			return true;
		}
	}
	else if(location + amount < 40 + player.boardOffset)//check if pawn reaches finish
	{
		return true;
	}
	else //pawn reaches finish
	{
		int finishLocation = (location + amount) - (40 + player.boardOffset);
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
	PlayerLogic *player = getPlayerByColorPointer(board[location]);
	bool reachesFinish = false;

	if (location + amount < player->boardOffset)
	{
		int offsetLocation = location + amount + (40 - player->boardOffset);
		if (offsetLocation >= 40)//check if pawn reaches finish
		{
			reachesFinish = true;
		}
	}
	else if (location + amount >= 40 + player->boardOffset)//check if pawn reaches finish
	{
		reachesFinish = true;
	}

	if (reachesFinish) //pawn reached finish
	{
		int finishLocation = (location + amount) - (40 + player->boardOffset);
		if (finishLocation > 3)
		{
			finishLocation = 3 - (finishLocation - 3);
		}
		player->finish[finishLocation] = player->playerColor;
		board[location] = state::empty;
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
	PlayerLogic player = PlayerLogic(empty, 0);
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
			if (player.finish[i] == empty)
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
		case empty:
			std::cout << "_";
			break;
		case blue:
			std::cout << "B";
			break;
		case red:
			std::cout << "R";
			break;
		case yellow:
			std::cout << "Y";
			break;
		case green:
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