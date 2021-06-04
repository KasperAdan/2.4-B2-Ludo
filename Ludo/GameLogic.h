#pragma once
#include "BoardLogic.h"
#include "PlayerLogic.h"
#include <vector>

class GameLogic
{
public:
	std::vector<PlayerLogic> players;

	GameLogic();
	GameLogic(int);
	~GameLogic();

private:

};
