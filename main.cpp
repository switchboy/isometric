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

void updateBuildingsHelper(int i)
{
    listOfBuildings[i].update();
}

void routeHelper(int i)
{
    listOfActors[i].calculateRoute();
}

void updateActorsWorker()
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

void updateObjectsWorker()
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

void updateBuildingsWorker()
{
    while(window.isOpen())
    {
        if(!listOfBuildings.empty())
        {
            for(int i = 0; i < listOfBuildings.size(); i++)
            {
                std::async(std::launch::async, updateBuildingsHelper, i);
            }
        }
    }
}

void calculateRoutesWorker()
{
    while(window.isOpen())
    {
        if(!listOfActors.empty())
        {
            for(int i = 0; i < listOfActors.size(); i++)
            {
                std::async(std::launch::async, routeHelper, i);
            }
        }
    }
}

int main()
{
    sf::Clock clockMain;
    currentGame.loadGame();
    std::thread updateActorsThread(updateActorsWorker);
    std::thread updateObjectsThread(updateObjectsWorker);
    std::thread updateBuildingsThread(updateBuildingsWorker);
    std::thread updateRoutesThread(calculateRoutesWorker);
    while(window.isOpen())
    {
        sf::Time elapsedMain = clockMain.getElapsedTime();
        currentGame.elapsedTime = elapsedMain.asSeconds();
        currentGame.interact();
        currentGame.drawGame();
    }
    updateActorsThread.join();
    updateObjectsThread.join();
    updateBuildingsThread.join();
    updateRoutesThread.join();
    return 0;
}



