#include "GameLogic.h"
#include <iostream>


GameLogic game;

int main(void)
{
    game = GameLogic(4);
    Dobble d = Dobble(0);
    int diceFound = d.findDice();
    std::cout << diceFound;

    diceFound = d.findDice();
    std::cout << diceFound;
    return 0;
}