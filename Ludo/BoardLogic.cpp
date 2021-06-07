#include "BoardLogic.h"

BoardLogic::BoardLogic()
{
}


BoardLogic::BoardLogic(int amountOfPlayers)
{
	for (int i = 0; i < amountOfPlayers; i++)
	{
		players.push_back(PlayerLogic((state)(1 + i), (10 * i)));
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

void BoardLogic::spawnPawn(state color)
{
	PlayerLogic player = getPlayerByColor(color);
	if (player.homePawns != 0)
	{
		player.removeHomePawn();
		if (board[player.boardOffset] != state::empty)
		{
			PlayerLogic enemy = getPlayerByColor(board[player.boardOffset]);
			enemy.addHomePawn();
		}
		board[player.boardOffset] = player.playerColor;
	}
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
void BoardLogic::movePawn(int location, int amount)
{
	PlayerLogic player = getPlayerByColor(board[location]);
	bool reachesFinish = false;

	if (location + amount < player.boardOffset)
	{
		int offsetLocation = location + amount + (40 - player.boardOffset);
		if (offsetLocation >= 40)//check if pawn reaches finish
		{
			reachesFinish = true;
		}
	}
	else if (location + amount >= 40 + player.boardOffset)//check if pawn reaches finish
	{
		reachesFinish = true;
	}

	if (reachesFinish) //pawn reached finish
	{
		int finishLocation = (location + amount) - (40 + player.boardOffset);
		if (finishLocation > 3)
		{
			finishLocation = 3 - (finishLocation - 3);
		}
		player.finish[finishLocation] = player.playerColor;
		board[location] = state::empty;
		return;
	}

	if (board[(location + amount) % 40] != state::empty)//check for an enemy on new position
	{
		state enemyColor = board[(location + amount) % 40];
		PlayerLogic enemy = getPlayerByColor(enemyColor);
		enemy.addHomePawn();
	}
	board[(location + amount) % 40] = player.playerColor;
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
