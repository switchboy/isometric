#include "actors.h"
#include "objects.h"
#include "player.h"
#include "buildings.h"
#include <iostream>

Cells cellsList[MAP_HEIGHT*MAP_WIDTH];

void Cells::updateCells(int goalX, int goalY)
{
    int n = 0;
    for(int i = 0; i < MAP_WIDTH; i++)
    {
        for(int j = 0; j < MAP_HEIGHT; j++)
        {
            cellsList[n].positionX = i;
            cellsList[n].positionY = j;
            if(n == (goalX*MAP_HEIGHT)+goalY)
            {
                cellsList[n].obstacle = false;
            }
            else if(!currentGame.isPassable(i, j))
            {
                cellsList[n].obstacle = true;
            }
            else
            {
                cellsList[n].obstacle = false;
            }
            cellsList[n].costToGoal = NULL;
            cellsList[n].visited = false;
            cellsList[n].parentCellId = NULL;
            cellsList[n].cummulativeCost = NULL;
            cellsList[n].totalCostGuess = NULL;
            cellsList[n].neighbours.clear();
            cellsList[n].cellId = n;
            n++;
        }
    }

    for(int i = 0; i < (MAP_HEIGHT*MAP_WIDTH) ; i++)
    {
        if(cellsList[i].positionX > 0)
        {
            //de cell links kan toegevoegd worden
            if(!cellsList[i-MAP_HEIGHT].obstacle)
            {
                cellsList[i].neighbours.push_back(i-MAP_HEIGHT);
            }
        }
        if(cellsList[i].positionX < MAP_WIDTH-1)
        {
            //de cell rechts kan toegevoegd worden
            if(!cellsList[i+MAP_HEIGHT].obstacle)
            {
                cellsList[i].neighbours.push_back(i+MAP_HEIGHT);
            }
        }
        if(cellsList[i].positionY > 0)
        {
            //de cell erboven kan toegevogd worden
            if(!cellsList[i-1].obstacle)
            {
                cellsList[i].neighbours.push_back(i-1);
            }
        }
        if(cellsList[i].positionY != MAP_HEIGHT-1)
        {
            //de cell eronder kan toegevoegd worden
            if(!cellsList[i+1].obstacle)
            {
                cellsList[i].neighbours.push_back(i+1);
            }
        }
        //schuin gaan...
        if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX < MAP_WIDTH-1)
        {
            //de cell rechtsonder kan toegevoegd worden
            if(!cellsList[i+1+MAP_HEIGHT].obstacle )
            {
                if(cellsList[i+1].obstacle  && cellsList[i+MAP_HEIGHT].obstacle)
                {
                    //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                }
                else
                {
                    cellsList[i].neighbours.push_back(i+1+MAP_HEIGHT);
                }
            }
        }
        if(cellsList[i].positionY >0 && cellsList[i].positionX < MAP_WIDTH-1)
        {
            //de cell rechtsboven kan toegevoegd worden
            if(!cellsList[i-1+MAP_HEIGHT].obstacle)
            {
                if(cellsList[i-1].obstacle && cellsList[i+MAP_HEIGHT].obstacle)
                {
                    //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                }
                else
                {
                    cellsList[i].neighbours.push_back(i-1+MAP_HEIGHT);
                }
            }
        }
        if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX > 0)
        {
            //de cell linksonder kan toegevoegd worden
            if(!cellsList[i+1-MAP_HEIGHT].obstacle)
            {
                if(cellsList[i+1].obstacle && cellsList[i-MAP_HEIGHT].obstacle)
                {
                    //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                }
                else
                {
                    cellsList[i].neighbours.push_back(i+1-MAP_HEIGHT);
                }
            }
        }
        if(cellsList[i].positionY >0 && cellsList[i].positionX > 0)
        {
            //de cell rechtsboven kan toegevoegd worden
            if(!cellsList[i-1-MAP_HEIGHT].obstacle)
            {
                if(cellsList[i-MAP_HEIGHT].obstacle && cellsList[i-1].obstacle)
                {
                    //Dit hokje wordt door de twee buur hokjes geblokkeerd!
                }
                else
                {
                    cellsList[i].neighbours.push_back(i-1-MAP_HEIGHT);
                }
            }
        }
    }
}

double dist(double x1, double y1, double x2, double y2)
{
    double x = x1 - x2; //calculating number to square in next step
    double y = y1 - y2;
    double dist;
    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);
    return dist;
}

std::vector<actors> listOfActors;

actors::actors(int type, int actorX, int actorY, int actorTeam, int actorId)
{
    this->actorType = type;
    this->actorCords[0] = actorX;
    this->actorCords[1] = actorY;
    this->actorGoal[0] = actorX;
    this->actorGoal[1] = actorY;
    this->actorTeam = actorTeam;
    this->actorId = actorId;
    this->orientation = 0;
    this->spriteYOffset = 0;
    this->retries = 0;
    this->amountOfFood = 0;
    this->amountOfGold = 0;
    this->amountOfStone = 0;
    this->amountOfWood = 0;
    this->timeLastAttempt = 1.0f;
    this->timeLastOffsetChange = 0.0f;
    this->timeStartedWalkingToRecource = 0.0f;
    this->goalNeedsUpdate = false;
    this->busyWalking = false;
    this->pathFound = false;
    this->isAtRecource = false;
    this->isGatheringRecources = false;
    this->hasToUnloadResource = false;
    this->isBackAtOwnSquare = false;
    this->offSetX = 0.0f;
    this->offSetY = 0.0f;
    currentGame.occupiedByActorList[actorX][actorY] = actorId;
    listOfPlayers[actorTeam].addToCurrentPopulation(1);
}

actors::~actors()
{
    //dtor
}


void actors::updateGoal(int i, int j, int waitTime)
{
    //check if values are in bounds
    if((i >= 0 && i < MAP_WIDTH) && (j >= 0 && j < MAP_HEIGHT))
    {
        currentGame.movedFromByActorList[this->actorCords[0]][this->actorCords[1]] = -1;
        this->actorGoal[0] = i;
        this->actorGoal[1] = j;
        this->waitForAmountOfFrames = waitTime;
        this->goalNeedsUpdate = true;
        this->busyWalking = false;
        this->pathFound = false;
        this->isAtRecource = false;
        this->offSetX = 0.0f;
        this->offSetY = 0.0f;
    }
}

int actorOrientation(int Xc, int Yc, int Xn, int Yn)
{

    //Orientation:
    //0 N       0   degrees     = x-1  y-1
    //1 NE      45  degrees     = x    y-1
    //2 E       90  degrees     = x+1  y-1
    //3 SE      135 degrees     = x+1  y
    //4 S       180 degrees     = x+1  y+1
    //5 SW      225 degrees     = x    y+1
    //6 W       270 degrees     = x-1  y+1
    //7 NW      315 degrees     = x-1  y
    int diffX = Xn-Xc;
    int diffY = Yn-Yc;

    switch(diffX)
    {
    case -1:
        switch(diffY)
        {
        case -1:
            return 0;
        case 0:
            return 7;
        case 1:
            return 6;
        }
    case 0:
        switch(diffY)
        {
        case -1:
            return 1;
        case 1:
            return 5;
        }
    case 1:
        switch(diffY)
        {
        case -1:
            return 2;
        case 0:
            return 3;
        case 1:
            return 4;
        }
    default:
        return 0;
    }
}

int newOrientation(int oldOrientation, int desiredOrientation)
{
    int differenceInOrientation = (oldOrientation + desiredOrientation)- desiredOrientation;
    int output;
    int amountOfStepsRight;
    int amountOfStepsLeft;
    //calcualte amount of tik's going right en left
    if(oldOrientation < desiredOrientation)
    {
        amountOfStepsRight = desiredOrientation - oldOrientation;
        amountOfStepsLeft = (oldOrientation+8) - desiredOrientation;
    }
    else if(oldOrientation > desiredOrientation)
    {
        amountOfStepsRight = (desiredOrientation+8) - oldOrientation;
        amountOfStepsLeft = oldOrientation - desiredOrientation;
    }
    else
    {
        int amountOfStepsRight = 0;
        int amountOfStepsLeft = 0;
    }
    if(amountOfStepsLeft < amountOfStepsRight)
    {
        output = oldOrientation -1;
    }
    else if (amountOfStepsLeft > amountOfStepsRight)
    {
        output = oldOrientation +1;
    }
    else if(amountOfStepsLeft == amountOfStepsRight && amountOfStepsLeft != 0)
    {
        output = oldOrientation +1;
    }
    else
    {
        output = oldOrientation;
    }
    if(output < 0)
    {
        output = 7;
    }
    if(output > 7)
    {
        output = 0;
    }
    return output;
}

void actors::update()
{
    if(this->goalNeedsUpdate)
    {
        this->pathFound = false;
        this->busyWalking = false;
        this->route.clear();
        this->retries = 0;
        if(this->waitForAmountOfFrames == 0)
        {
            this->calculateRoute();
            this->goalNeedsUpdate = false;
        }
        else
        {
            this->waitForAmountOfFrames += -1;
        }
    }
    else
    {
        if(this->busyWalking && (currentGame.elapsedTime-this->timeLastUpdate) > 1.0f)
        {
            this->busyWalking = false;
            this->movedMoreThanHalf = false;
            this->actorCords[0] = this->actorGoal[0];
            this->actorCords[1] = this->actorGoal[1];
            currentGame.movedFromByActorList[this->actorCords[0]][this->actorCords[1]] = -1;
        }
        else if(this->busyWalking && (currentGame.elapsedTime-this->timeLastUpdate) > 0.5f && !this->movedMoreThanHalf)
        {
            currentGame.occupiedByActorList[this->actorCords[0]][this->actorCords[1]] = -1;
            currentGame.movedFromByActorList[this->actorCords[0]][this->actorCords[1]] = this->actorId;
            this->movedMoreThanHalf = true;
        }

        if((!this->busyWalking) && this->pathFound &&(!this->route.empty()))
        {
            //Deze actor heeft een doel, dit doel is nog niet bereikt en is klaar met het vorige stuk lopen!
            int wantedOrientation = actorOrientation(this->actorCords[0], this->actorCords[1], this->route.back().positionX, this->route.back().positionY);
            if(wantedOrientation == this->orientation)
            {
                if(this->isGatheringRecources && this->route.size() == 1)
                {
                    this->pathFound = false;
                    this->commonGoal = false;
                    this->route.clear();
                    if(this->isWalkingToUnloadingPoint)
                    {
                        this->reachedUnloadingPoint = true;
                    }
                }
                else
                {
                    //De actor staat met zijn neus de goede kant op en kan dus gaan lopen als de tegel vrij is!
                    if(currentGame.occupiedByActorList[this->route.back().positionX][this->route.back().positionY] == -1)
                    {
                        this->retries = 0;
                        this->busyWalking = true;
                        this->timeLastUpdate = currentGame.elapsedTime;
                        this->actorGoal[0] = this->route.back().positionX;
                        this->actorGoal[1] = this->route.back().positionY;
                        currentGame.occupiedByActorList[this->route.back().positionX][this->route.back().positionY] = this->actorId;
                        this->route.pop_back();
                        if(route.empty())
                        {
                            this->pathFound = false;
                            this->commonGoal = false;
                            if(this->isWalkingToUnloadingPoint)
                            {
                                this->reachedUnloadingPoint = true;
                            }
                        }
                    }
                    else if(retries < 5)
                    {
                        if(currentGame.elapsedTime-this->timeLastAttempt > 1)
                        {
                            //there is a problem; do nothing this frame but calculate an alternative route!
                            this->actorGoal[0] = this->route.front().positionX;
                            this->actorGoal[1] = this->route.front().positionY;
                            this->calculateRoute();
                            this->retries += +1;
                            this->timeLastAttempt = currentGame.elapsedTime;
                        }
                    }
                    else
                    {
                        this->pathFound = false;
                        this->route.clear();
                        this->commonGoal = false;
                        this->retries = 0;
                    }
                }
            }
            else
            {
                //De actor moet eerst draaien voordat hij kan gaan lopen
                this->orientation = newOrientation(this->orientation, wantedOrientation);
            }
        }
        else if(this->isGatheringRecources && (!this->busyWalking)&& this->route.empty())
        {
            int northSouth;
            int eastWest;
            int diagonalX;
            int diagonalY;
            if(this->ResourceBeingGatherd == 0)
            {
                northSouth = 22;
                eastWest = 55;
                diagonalX = 21;
                diagonalY = 12;
            }
            else
            {
                northSouth = 11;
                eastWest = 27;
                diagonalX = 11;
                diagonalY = 6;
            }
            if(this->hasToUnloadResource)
            {
                if(!this->isBackAtOwnSquare)
                {
                    if(this->timeStartedWalkingToRecource == 0.0f)
                    {
                        this->timeStartedWalkingToRecource = currentGame.elapsedTime;
                    }
                    else if(currentGame.elapsedTime - this->timeStartedWalkingToRecource < 0.5f)
                    {
                        //0 N       0   degrees     = x-1  y-1
                        //1 NE      45  degrees     = x    y-1
                        //2 E       90  degrees     = x+1  y-1
                        //3 SE      135 degrees     = x+1  y
                        //4 S       180 degrees     = x+1  y+1
                        //5 SW      225 degrees     = x    y+1
                        //6 W       270 degrees     = x-1  y+1
                        //7 NW      315 degrees     = x-1  y
                        switch(this->orientation)
                        {
                        case 0:
                            this->offSetX = 0;
                            this->offSetY = -northSouth+((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*northSouth;
                            break;
                        case 1:
                            this->offSetX = diagonalX-((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX;
                            this->offSetY = -diagonalY+((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY;
                            break;
                        case 2:
                            this->offSetX = eastWest-((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*eastWest;
                            this->offSetY = 0;
                            break;
                        case 3:
                            this->offSetX = diagonalX-((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX;
                            this->offSetY = diagonalY-((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY;
                            break;
                        case 4:
                            this->offSetX = 0;
                            this->offSetY = northSouth-((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*northSouth;
                            break;
                        case 5:
                            this->offSetX = -diagonalX+((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX;
                            this->offSetY = diagonalY-((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY;
                            break;
                        case 6:
                            this->offSetX = -eastWest+((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*eastWest;
                            this->offSetY = 0;
                            break;
                        case 7:
                            this->offSetX = -diagonalX+((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX;
                            this->offSetY = -diagonalY+((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY;
                            break;
                        }
                    }
                    else
                    {
                        this->isBackAtOwnSquare = true;
                        this->isAtRecource = false;
                        this->timeStartedWalkingToRecource = 0.0f;
                        this->timeStartedGatheringRecource = currentGame.elapsedTime;
                        this->offSetX = 0;
                        this->offSetY = 0;
                    }


                }
                else
                {
                    if(!this->isWalkingToUnloadingPoint)
                    {
                        //search the nearest unloading point an walk there
                        this->dropOffTile = findNearestDropOffPoint(this->ResourceBeingGatherd);
                        if(this->dropOffTile.isSet)
                        {
                            updateGoal(this->dropOffTile.locationX, this->dropOffTile.locationY, 0);
                            this->isWalkingToUnloadingPoint = true;
                            this->reachedUnloadingPoint = false;
                        }
                        else
                        {
                            //There is no dropoff point!
                        }
                    }
                    else if(this->reachedUnloadingPoint)
                    {
                        switch(listOfBuildings[this->dropOffTile.buildingId].getRecievesWhichResources())
                        {
                        case 0:
                            //recieves only wood
                            listOfPlayers[this->actorTeam].addResources(0, this->amountOfWood);
                            this->amountOfWood = 0;
                            break;
                        case 1:
                            //recieves only food
                            listOfPlayers[this->actorTeam].addResources(1, this->amountOfFood);
                            this->amountOfFood = 0;
                            break;
                        case 2:
                            //recieves only stone
                            listOfPlayers[this->actorTeam].addResources(2, this->amountOfStone);
                            this->amountOfStone = 0;
                            break;
                        case 3:
                            //recieves only gold
                            listOfPlayers[this->actorTeam].addResources(3, this->amountOfGold);
                            this->amountOfGold = 0;
                            break;
                        case 4:
                            //recieves all the resources!
                            listOfPlayers[this->actorTeam].addResources(0, this->amountOfWood);
                            this->amountOfWood = 0;
                            listOfPlayers[this->actorTeam].addResources(1, this->amountOfFood);
                            this->amountOfFood = 0;
                            listOfPlayers[this->actorTeam].addResources(2, this->amountOfStone);
                            this->amountOfStone = 0;
                            listOfPlayers[this->actorTeam].addResources(3, this->amountOfGold);
                            this->amountOfGold = 0;
                            break;
                        }
                        if(currentGame.objectLocationList[this->gatheringResourcesAt[0]][this->gatheringResourcesAt[1]] != -1)
                        {
                            this->updateGoal(this->gatheringResourcesAt[0], this->gatheringResourcesAt[1], 0);
                            this->isWalkingToUnloadingPoint = false;
                            this->isAtCarryCapacity = false;
                            this->carriesRecources = false;
                            this->isAtRecource = false;
                            this->hasToUnloadResource = false;
                            this->timeStartedWalkingToRecource = 0.0f;
                        }
                        else
                        {
                            //vind binnen 20 tegels de dichtbijzijnde van dezelfde resource
                            int lowSearchLimitX = this->actorCords[0]-10;
                            if(lowSearchLimitX < 0)
                            {
                                lowSearchLimitX = 0;
                            }
                            int lowSearchLimitY = this->actorCords[1]-10;
                            if(lowSearchLimitY < 0)
                            {
                                lowSearchLimitY = 0;
                            }
                            int highSearchLimitX = this->actorCords[0]+10;
                            if(highSearchLimitX > MAP_WIDTH)
                            {
                                highSearchLimitX = MAP_WIDTH;
                            }
                            int highSearchLimitY = this->actorCords[0]+10;
                            if(highSearchLimitY > MAP_HEIGHT)
                            {
                                highSearchLimitY = MAP_HEIGHT;
                            }
                            nearestBuildingTile nearestObject;
                            nearestObject = {0,0,0,0,false};
                            for(int i = lowSearchLimitX; i < highSearchLimitX; i++)
                            {
                                for(int j = lowSearchLimitY; j < highSearchLimitY; j++)
                                {
                                    if(currentGame.objectLocationList[i][j] != -1)
                                    {
                                        if(listOfObjects[currentGame.objectLocationList[i][j]].getTypeOfResource() == this->ResourceBeingGatherd)
                                        {
                                            float tempDeltaDistance = dist(this->actorCords[0], this->actorCords[1], i, j);
                                            if(!nearestObject.isSet)
                                            {
                                                nearestObject = {tempDeltaDistance, i, j,currentGame.objectLocationList[i][j], true};
                                            }
                                            else if(tempDeltaDistance < nearestObject.deltaDistance)
                                            {
                                                nearestObject = {tempDeltaDistance, i, j,currentGame.objectLocationList[i][j], true};
                                            }
                                        }
                                    }
                                }
                            }
                            if(nearestObject.isSet)
                            {
                                this->gatheringResourcesAt[0] = nearestObject.locationX;
                                this->gatheringResourcesAt[1] = nearestObject.locationY;
                            }
                            else
                            {
                                this->isGatheringRecources = false;
                            }
                        }
                    }
                }
            }
            else
            {
                //verzamel recources
                if(this->isAtRecource)
                {
                    if(currentGame.objectLocationList[this->gatheringResourcesAt[0]][this->gatheringResourcesAt[1]] != -1)
                    {
                        if(currentGame.elapsedTime - this->timeStartedGatheringRecource > 2)
                        {
                            switch(this->ResourceBeingGatherd)
                            {
                            case 0:  //wood
                                this->amountOfWood += +1;
                                break;
                            case 1: //food
                                this->amountOfFood += +1;
                                break;
                            case 2: //stone
                                this->amountOfStone += +1;
                                break;
                            case 3: // gold
                                this->amountOfGold += +1;
                                break;
                            }
                            listOfObjects[currentGame.objectLocationList[this->gatheringResourcesAt[0]][this->gatheringResourcesAt[1]]].substractResource();
                            this->carriesRecources = true;
                            if((this->amountOfFood == 10) || (this->amountOfWood == 10) ||(this->amountOfStone == 10) ||(this->amountOfGold == 10) )
                            {
                                this->hasToUnloadResource = true;
                                this->isAtRecource = false;
                            }
                            this->timeStartedGatheringRecource = currentGame.elapsedTime;
                        }
                    }
                    else
                    {
                        //resource not here!
                        this->hasToUnloadResource = true;
                        this->isAtRecource = false;
                    }
                }
                else if(this->timeStartedWalkingToRecource == 0.0f)
                {
                    this->timeStartedWalkingToRecource = currentGame.elapsedTime;
                }
                else if(currentGame.elapsedTime - this->timeStartedWalkingToRecource < 0.5f)
                {
                    //0 N       0   degrees     = x-1  y-1
                    //1 NE      45  degrees     = x    y-1
                    //2 E       90  degrees     = x+1  y-1
                    //3 SE      135 degrees     = x+1  y
                    //4 S       180 degrees     = x+1  y+1
                    //5 SW      225 degrees     = x    y+1
                    //6 W       270 degrees     = x-1  y+1
                    //7 NW      315 degrees     = x-1  y
                    switch(this->orientation)
                    {
                    case 0:
                        this->offSetX = 0;
                        this->offSetY = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*northSouth*-1;
                        break;
                    case 1:
                        this->offSetX = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX;
                        this->offSetY = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY*-1;
                        break;
                    case 2:
                        this->offSetX = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*eastWest;
                        this->offSetY = 0;
                        break;
                    case 3:
                        this->offSetX = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX;
                        this->offSetY = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY;
                        break;
                    case 4:
                        this->offSetX = 0;
                        this->offSetY = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*northSouth;
                        break;
                    case 5:
                        this->offSetX = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX*-1;
                        this->offSetY = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY;
                        break;
                    case 6:
                        this->offSetX = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*eastWest*-1;
                        this->offSetY = 0;
                        break;
                    case 7:
                        this->offSetX = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalX*-1;
                        this->offSetY = ((currentGame.elapsedTime - this->timeStartedWalkingToRecource) / 0.5f)*diagonalY*-1;
                        break;
                    }
                }
                else
                {
                    this->isAtRecource = true;
                    this->timeStartedWalkingToRecource = 0.0f;
                    this->timeStartedGatheringRecource = currentGame.elapsedTime;
                    switch(this->orientation)
                    {
                    case 0:
                        this->offSetX = 0;
                        this->offSetY = -northSouth;
                        break;
                    case 1:
                        this->offSetX = diagonalX;
                        this->offSetY = -diagonalY;
                        break;
                    case 2:
                        this->offSetX = eastWest;
                        this->offSetY = 0;
                        break;
                    case 3:
                        this->offSetX = diagonalX;
                        this->offSetY = diagonalY;
                        break;
                    case 4:
                        this->offSetX = 0;
                        this->offSetY = northSouth;
                        break;
                    case 5:
                        this->offSetX = -diagonalX;
                        this->offSetY = diagonalY;
                        break;
                    case 6:
                        this->offSetX = -eastWest;
                        this->offSetY = 0;
                        break;
                    case 7:
                        this->offSetX = -diagonalX;
                        this->offSetY = -diagonalY;
                        break;

                    }
                }
            }
        }
        if(commonGoal && !pathFound && retries < 6 && currentGame.elapsedTime-this->timeLastAttempt > 1)
        {
            this->calculateRoute();
            this->retries += +1;
            this->timeLastAttempt = currentGame.elapsedTime;
        }
        if(this->retries == 5)
        {
            this->pathFound = false;
            this->route.clear();
            this->commonGoal = false;
            this->retries = 0;
        }
    }
}

void actors::setGatheringRecource(bool flag)
{
    this->isGatheringRecources = flag;
    if(flag)
    {
        this->ResourceBeingGatherd = listOfObjects[currentGame.objectLocationList[this->actorGoal[0]][this->actorGoal[1]]].getTypeOfResource();
        this->gatheringResourcesAt[0] = this->actorGoal[0];
        this->gatheringResourcesAt[1] = this->actorGoal[1];
    }
}

void actors::setCommonGoalTrue()
{
    this->commonGoal = true;
}

nearestBuildingTile actors::findNearestDropOffPoint(int Resource)
{
    nearestBuildingTile nearestDropOffBuilding;
    nearestDropOffBuilding = {0,0,0,0,false};
    for(int i = 0; i < listOfBuildings.size(); i++)
    {
        if((listOfBuildings[i].getRecievesWhichResources() == Resource || listOfBuildings[i].getRecievesWhichResources() == 4)
                && listOfBuildings[i].getTeam() == this->actorTeam)
        {
            for(int x = 0; x < footprintOfBuildings[listOfBuildings[i].getType()].amountOfXFootprint; x++)
            {
                for(int y = 0; y < footprintOfBuildings[listOfBuildings[i].getType()].amountOfYFootprint; y++)
                {
                    int tempX = listOfBuildings[i].getLocationX()-x;
                    int tempY = listOfBuildings[i].getLocationY()-y;
                    float tempDeltaDistance = dist(this->actorCords[0], this->actorCords[1], tempX, tempY);
                    if(!nearestDropOffBuilding.isSet)
                    {
                        nearestDropOffBuilding = {tempDeltaDistance, tempX, tempY, listOfBuildings[i].getBuildingId(), true};
                    }
                    else if(tempDeltaDistance < nearestDropOffBuilding.deltaDistance)
                    {
                        nearestDropOffBuilding = {tempDeltaDistance, tempX, tempY, listOfBuildings[i].getBuildingId(), true};
                    }
                }
            }
        }
    }
    return nearestDropOffBuilding;
}

void actors::calculateRoute()
{
    cellsList->updateCells(actorGoal[0], actorGoal[1]);
    std::list<Cells> listToCheck;
    std::list<Cells> checkedList;
    int startCell = (actorCords[0]*MAP_HEIGHT)+actorCords[1]; //eigen positie
    int endCell = (actorGoal[0]*MAP_HEIGHT)+actorGoal[1]; //doel positie
    bool endReached = false;

    //check of de doelcel niet 1 hokje weg is
    if(((actorCords[0]-actorGoal[0] == 0) ||(actorCords[0]-actorGoal[0] == -1) ||(actorCords[0]-actorGoal[0] == 1)) && ((actorCords[1]-actorGoal[1] == 0) ||(actorCords[1]-actorGoal[1] == -1) ||(actorCords[1]-actorGoal[1] == 1)))
    {
        if(!cellsList[endCell].obstacle)
        {
            this->pathFound = true;
            endReached = true;
        }
        else
        {
            this->pathFound = false;
        }
    }
    else
    {
        listToCheck.push_back(cellsList[startCell]);
        this->pathFound = false;
        cellsList[startCell].visited = true;
    }

    while(!listToCheck.empty() && startCell != endCell)
    {
        //sorteer de lijst en zet de cell met de laagste cost to goal bovenaan om het eerst te testen
        listToCheck.sort([](const Cells &f, const Cells &s)
        {
            return f.totalCostGuess < s.totalCostGuess;
        });
        //Check of de te checken cell het doel is. Als dat zo is zijn we klaar
        if(listToCheck.front().cellId == endCell)
        {
            listToCheck.clear();
            this->pathFound = true;
        }
        else
        {
            for (std::list<int>::const_iterator iterator =  listToCheck.front().neighbours.begin(), end =  listToCheck.front().neighbours.end(); iterator != end; ++iterator)
            {
                //We have found neighbours!
                //check if neighbours was found before
                if(!cellsList[*iterator].visited)
                {
                    //Deze cell heeft geen parent is is dus nooit eerder gevonden!
                    //De cell waarvan we de neighbours onderzoeken is dus automagisch tot nu toe de kortste route hiernaartoe
                    cellsList[*iterator].parentCellId = listToCheck.front().cellId;
                    //Nu moeten de kosten voor de route hiernatoe uitgerekend worden (Dit zijn de kosten van naar de buurman gaan +1
                    cellsList[*iterator].cummulativeCost = listToCheck.front().cummulativeCost+1;
                    //Als laatste zetten we de cell in de lijst met cellen die gecheckt moet worden
                    listToCheck.push_back(cellsList[*iterator]);
                    //Bereken de afstand naar het doel
                    cellsList[*iterator].costToGoal = dist(cellsList[*iterator].positionX,cellsList[*iterator].positionY,cellsList[endCell].positionX,cellsList[endCell].positionY);
                    cellsList[*iterator].totalCostGuess = cellsList[*iterator].costToGoal + cellsList[*iterator].cummulativeCost;
                    cellsList[*iterator].visited = true;
                }
                else
                {
                    //Deze cell is al eerder gevonden, staat dus al in de te checken cell lijst
                    if(listToCheck.front().cummulativeCost+1 < cellsList[*iterator].cummulativeCost)
                    {
                        //Er is een kortere route naar deze cell! Pas de parent cell dus aan en geef een nieuwe cummulative Cost;
                        cellsList[*iterator].parentCellId = listToCheck.front().cellId;
                        cellsList[*iterator].cummulativeCost = listToCheck.front().cummulativeCost+1;
                        cellsList[*iterator].totalCostGuess = cellsList[*iterator].costToGoal + cellsList[*iterator].cummulativeCost;
                    }
                }
            }
            checkedList.push_back(listToCheck.front());
            listToCheck.pop_front();
        }
    }
    //Zet de te lopen route in een lijst
    this->route.clear();
    this->route.push_back(cellsList[endCell]);
    if(this->pathFound)
    {
        while(!endReached)
        {
            if(route.back().visited == true)
            {
                this->route.push_back(cellsList[route.back().parentCellId]);
                if(this->route.back().parentCellId == startCell)
                {
                    endReached = true;
                }
            }
            else
            {
                endReached = true;
            }
        }
    }
}

void actors:: drawActor()
{
    int i = this->actorCords[0];
    int j = this->actorCords[1];
    int x = this->actorGoal[0];
    int y = this->actorGoal[1];
    int xPosition = worldSpace(i,j,true);
    int yPosition = worldSpace(i,j,false);
    int spriteXoffset = 0;
    int spriteYoffset = 0;
    if(this->busyWalking || this->timeStartedWalkingToRecource > 0.0f)
    {
        if(currentGame.elapsedTime-this->timeLastOffsetChange > 0.2f)
        {
            this->spriteYOffset = this->spriteYOffset + 32;
            if(this->spriteYOffset > 128)
            {
                this->spriteYOffset = 32;
            }
            this->timeLastOffsetChange = currentGame.elapsedTime;
        }

        int nXPosition = worldSpace(x,y,true);
        int nYPosition = worldSpace(x,y,false);
        int deltaX = xPosition-nXPosition;
        int deltaY = yPosition-nYPosition;
        float deltaTime = currentGame.elapsedTime - this->timeLastUpdate;
        float deltaXCompleted = deltaX*deltaTime;
        float deltaYCompleted = deltaY*deltaTime;
        xPosition = xPosition - deltaXCompleted;
        yPosition = yPosition - deltaYCompleted;
    }
    else if(this->isAtRecource)
    {
        if(currentGame.elapsedTime-this->timeLastOffsetChange > 0.2f)
        {
            this->spriteYOffset = this->spriteYOffset + 32;
            if(this->spriteYOffset > 128)
            {
                this->spriteYOffset = 0;
            }
            this->timeLastOffsetChange = currentGame.elapsedTime;
        }
    }
    else
    {
        this->spriteYOffset = 0;
    }
    switch(this->spriteYOffset)
    {
    case 0:
        spriteYoffset = 0;
        break;
    case 32:
        spriteYoffset = 32;
        break;
    case 64:
        spriteYoffset = 64;
        break;
    case 96:
        spriteYoffset = 96;
        break;
    case 128:
        spriteYoffset = 128;
        break;
    }
    if(this->isAtRecource && this->ResourceBeingGatherd == 0)
    {
        switch(this->orientation)
        {
        case 0:
            spriteXoffset = 208;
            break;
        case 1:
            spriteXoffset = 128;
            break;
        case 2:
            spriteXoffset = 224;
            break;
        case 3:
            spriteXoffset = 176;
            break;
        case 4:
            spriteXoffset = 192;
            break;
        case 5:
            spriteXoffset = 160;
            break;
        case 6:
            spriteXoffset = 240;
            break;
        case 7:
            spriteXoffset = 144;
            break;
        default:
            spriteXoffset = 128;
            break;
        }
    }
    else
    {
        switch(this->orientation)
        {
        case 0:
            spriteXoffset = 80;
            break;
        case 1:
            spriteXoffset = 0;
            break;
        case 2:
            spriteXoffset = 96;
            break;
        case 3:
            spriteXoffset = 48;
            break;
        case 4:
            spriteXoffset = 64;
            break;
        case 5:
            spriteXoffset = 32;
            break;
        case 6:
            spriteXoffset = 112;
            break;
        case 7:
            spriteXoffset = 16;
            break;
        default:
            spriteXoffset = 0;
            break;
        }
    }

    xPosition = xPosition + this->offSetX;
    yPosition = yPosition + this->offSetY;
    currentGame.spriteVillager.setPosition(xPosition, yPosition);
    currentGame.spriteVillager.setTextureRect(sf::IntRect(spriteXoffset,spriteYoffset,16,32));
    window.draw(currentGame.spriteVillager);
}


void actors::renderPath()
{
    std::list<Cells>::iterator it;
    for(it = route.begin(); it!=route.end(); it++)
    {
        currentGame.spriteSelectedTileForPath.setPosition(worldSpace(it->positionX, it->positionY, true), worldSpace(it->positionX, it->positionY, false));
        window.draw(currentGame.spriteSelectedTileForPath);
    }
}
