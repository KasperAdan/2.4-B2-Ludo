#include "PlayerLogic.h"

PlayerLogic::PlayerLogic()
{
}

PlayerLogic::PlayerLogic(state color, int boardOffset)
{
	playerColor = color;
	boardOffset = boardOffset;
}

PlayerLogic::~PlayerLogic()
{
}

void PlayerLogic::addHomePawn()
{
	homePawns++;
}

void PlayerLogic::removeHomePawn()
{
	homePawns--;
}
