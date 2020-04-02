#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "main.h"
#include "animation.h"
#include "globals.h"
#include "actors.h"
#include "buildings.h"

gameState currentGame;

int main()
{
    sf::Clock clockMain;
    currentGame.loadGame();
    while(window.isOpen())
    {
        sf::Time elapsedMain = clockMain.getElapsedTime();
        currentGame.elapsedTime = elapsedMain.asSeconds();
        currentGame.interact();
        currentGame.update();
        currentGame.drawGame();
    }
    return 0;
}
