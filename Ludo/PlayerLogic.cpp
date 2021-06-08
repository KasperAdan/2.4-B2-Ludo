#include "PlayerLogic.h"

PlayerLogic::PlayerLogic()
{
}

PlayerLogic::PlayerLogic(state color, int offset)
{
	playerColor = color;
	boardOffset = offset;
	homePawns = 4;
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
