#include "BoardLogic.h"

BoardLogic::BoardLogic()
{
	for (int i = 0; i < 40; i++)
	{
		board[i] = state::empty;
	}
}

BoardLogic::~BoardLogic()
{
}