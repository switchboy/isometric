#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include "main.h"
#include "animation.h"
#include "globals.h"
#include "actors.h"
#include "buildings.h"
#include "objects.h"
#include <future>

gameState currentGame;

void updateActorHelper(int i)
{
    listOfActors[i].update();
}

void updateObjectHelper(int i)
{
    listOfObjects[i].update();
}

void updateActors()
{
    while(window.isOpen())
    {
        if(!listOfActors.empty())
        {
            for(int i = 0; i < listOfActors.size(); i++)
            {
                std::async(std::launch::async, updateActorHelper, i);
            }
        }
    }
}
void updateObjects()
{
    while(window.isOpen())
    {
        if(!listOfObjects.empty())
        {
            for(int i = 0; i < listOfObjects.size(); i++)
            {
                std::async(std::launch::async, updateObjectHelper, i);
            }
        }
    }
}

int main()
{
    sf::Clock clockMain;
    currentGame.loadGame();
    std::thread updateActorsThread(updateActors);
    std::thread updateObjectsThread(updateObjects);
    while(window.isOpen())
    {
        sf::Time elapsedMain = clockMain.getElapsedTime();
        currentGame.elapsedTime = elapsedMain.asSeconds();
        currentGame.interact();

        currentGame.drawGame();
    }
    return 0;
}



