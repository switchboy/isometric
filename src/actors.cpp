#include "actors.h"
#include "objects.h"
#include "player.h"
#include "buildings.h"
#include <iostream>
#include <future>
#include <mutex>

void updateCells(int goalX, int goalY, std::vector<Cells>& cellsList)
{
    int goalId = (goalX*MAP_HEIGHT)+goalY;
    int n = 0;
    for(int i = 0; i < MAP_WIDTH; i++)
    {
        for(int j = 0; j < MAP_HEIGHT; j++)
        {
            cellsList[n].positionX = i;
            cellsList[n].positionY = j;
            if(n == goalId)
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
            for(int q = 0; q <8; q++)
            {
                cellsList[n].neighbours[q] = -1;
            }
            cellsList[n].cellId = n;
            n++;
        }
    }
}


void addNeighbours(int& i, std::vector<Cells>& cellsList)
{
    if(cellsList[i].positionX > 0)
    {
        //de cell links kan toegevoegd worden
        if(!cellsList[i-MAP_HEIGHT].obstacle)
        {
            cellsList[i].neighbours[0] = i-MAP_HEIGHT;
        }
    }
    if(cellsList[i].positionX < MAP_WIDTH-1)
    {
        //de cell rechts kan toegevoegd worden
        if(!cellsList[i+MAP_HEIGHT].obstacle)
        {
            cellsList[i].neighbours[1] =i+MAP_HEIGHT;
        }
    }
    if(cellsList[i].positionY > 0)
    {
        //de cell erboven kan toegevogd worden
        if(!cellsList[i-1].obstacle)
        {
            cellsList[i].neighbours[2] =i-1;
        }
    }
    if(cellsList[i].positionY != MAP_HEIGHT-1)
    {
        //de cell eronder kan toegevoegd worden
        if(!cellsList[i+1].obstacle)
        {
            cellsList[i].neighbours[3] =i+1;
        }
    }
    //schuin gaan...
    if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX < MAP_WIDTH-1)
    {
        //de cell rechtsonder kan toegevoegd worden
        if(!cellsList[i+1+MAP_HEIGHT].obstacle )
        {
            if(!(cellsList[i+1].obstacle  && cellsList[i+MAP_HEIGHT].obstacle))
            {
                cellsList[i].neighbours[4] =i+1+MAP_HEIGHT;
            }
        }
    }
    if(cellsList[i].positionY >0 && cellsList[i].positionX < MAP_WIDTH-1)
    {
        //de cell rechtsboven kan toegevoegd worden
        if(!cellsList[i-1+MAP_HEIGHT].obstacle)
        {
            if(!(cellsList[i-1].obstacle && cellsList[i+MAP_HEIGHT].obstacle))
            {
                cellsList[i].neighbours[5] =i-1+MAP_HEIGHT;
            }
        }
    }
    if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX > 0)
    {
        //de cell linksonder kan toegevoegd worden
        if(!cellsList[i+1-MAP_HEIGHT].obstacle)
        {
            if(!(cellsList[i+1].obstacle && cellsList[i-MAP_HEIGHT].obstacle))
            {
                cellsList[i].neighbours[6] =i+1-MAP_HEIGHT;
            }
        }
    }
    if(cellsList[i].positionY >0 && cellsList[i].positionX > 0)
    {
        //de cell rechtsboven kan toegevoegd worden
        if(!cellsList[i-1-MAP_HEIGHT].obstacle)
        {
            if(!(cellsList[i-MAP_HEIGHT].obstacle && cellsList[i-1].obstacle))
            {
                cellsList[i].neighbours[7] =i-1-MAP_HEIGHT;
            }
        }
    }

}

double dist(double x1, double y1, double x2, double y2)
{
////calculating Euclidean distance
//    double x = x1 - x2; //calculating number to square in next step
//    double y = y1 - y2;
//    double dist;
//    dist = pow(x, 2) + pow(y, 2);
//    dist = sqrt(dist);


//Manhattan distance
    double dist;
    int x_dif, y_dif;
    x_dif = x2 - x1;
    y_dif = y2 - y1;
    if(x_dif < 0)
    {
        x_dif = -x_dif;
    }
    if(y_dif < 0)
    {
        y_dif = -y_dif;
    }
    dist = x_dif + y_dif;
    return dist;
}



bool actors::canTargetBeReached()
{
    int sourceX = this->actorCords[0];
    int sourceY = this->actorCords[1];
    int targetX = this->actorGoal[0];
    int targetY = this->actorGoal[1];
    int maxCellId =  MAP_HEIGHT*MAP_WIDTH;
    int cellsList[maxCellId][2];
    int startCell = (sourceX*MAP_HEIGHT)+sourceY;
    int endCell = (targetX*MAP_HEIGHT)+targetY;
    int collisionCellId = 0;
    for(int i = 0; i < maxCellId; i++)
    {
        cellsList[i][0] = 0;
        cellsList[i][1] = 0;
    }
    cellsList[startCell][0] = 1;
    cellsList[endCell][1] = 1;
    islandCell checkedListFromSource[maxCellId];
    std::list <islandCell> toCheckListFromSource;
    islandCell checkedListFromTarget[maxCellId];
    std::list <islandCell> toCheckListFromTarget;
    toCheckListFromSource.push_back({sourceX, sourceY, startCell, 0, 0});
    toCheckListFromTarget.push_back({targetX, targetY, endCell, 0, 0});
    bool cellIsInBothLists = false;
    int cellScore = 0;
    while(!toCheckListFromSource.empty() && !toCheckListFromTarget.empty() && !cellIsInBothLists)
    {
        for(int i = 0; i < 8; i++)
        {
            int tempSourceCellId;
            int tempSourceX;
            int tempSourceY;
            int tempTargetCellId;
            int tempTargetX;
            int tempTargetY;
            switch(i)
            {
            case 0:
                //north
                tempSourceX = toCheckListFromSource.front().positionX;
                tempSourceY = toCheckListFromSource.front().positionY-1;
                tempTargetX = toCheckListFromTarget.front().positionX;
                tempTargetY = toCheckListFromTarget.front().positionY-1;
                break;
            case 1:

                //NorthEast
                tempSourceX = toCheckListFromSource.front().positionX+1;
                tempSourceY = toCheckListFromSource.front().positionY-1;
                tempTargetX = toCheckListFromTarget.front().positionX+1;
                tempTargetY = toCheckListFromTarget.front().positionY-1;
                break;
            case 2:
                //East
                tempSourceX = toCheckListFromSource.front().positionX+1;
                tempSourceY = toCheckListFromSource.front().positionY;
                tempTargetX = toCheckListFromTarget.front().positionX+1;
                tempTargetY = toCheckListFromTarget.front().positionY;
                break;
            case 3:
                //SouthEast
                tempSourceX = toCheckListFromSource.front().positionX+1;
                tempSourceY = toCheckListFromSource.front().positionY+1;
                tempTargetX = toCheckListFromTarget.front().positionX+1;
                tempTargetY = toCheckListFromTarget.front().positionY+1;
                break;
            case 4:
                //South
                tempSourceX = toCheckListFromSource.front().positionX;
                tempSourceY = toCheckListFromSource.front().positionY+1;
                tempTargetX = toCheckListFromTarget.front().positionX;
                tempTargetY = toCheckListFromTarget.front().positionY+1;
                break;
            case 5:
                //SouthWest
                tempSourceX = toCheckListFromSource.front().positionX-1;
                tempSourceY = toCheckListFromSource.front().positionY+1;
                tempTargetX = toCheckListFromTarget.front().positionX-1;
                tempTargetY = toCheckListFromTarget.front().positionY+1;
                break;
            case 6:
                //West
                tempSourceX = toCheckListFromSource.front().positionX-1;
                tempSourceY = toCheckListFromSource.front().positionY;
                tempTargetX = toCheckListFromTarget.front().positionX-1;
                tempTargetY = toCheckListFromTarget.front().positionY;
                break;
            case 7:
                //NorthWest
                tempSourceX = toCheckListFromSource.front().positionX-1;
                tempSourceY = toCheckListFromSource.front().positionY-1;
                tempTargetX = toCheckListFromTarget.front().positionX-1;
                tempTargetY = toCheckListFromTarget.front().positionY-1;
                break;
            }
            tempSourceCellId = (tempSourceX*MAP_HEIGHT)+tempSourceY;
            tempTargetCellId = (tempTargetX*MAP_HEIGHT)+tempTargetY;
            if(tempSourceCellId >= 0 && tempSourceCellId < maxCellId)
            {
                if(currentGame.isPassable(tempSourceX,tempSourceY))
                {
                    if(cellsList[tempSourceCellId][0] == 0)
                    {
                        toCheckListFromSource.push_back({tempSourceX, tempSourceY, tempSourceCellId, cellScore, toCheckListFromSource.front().cellId});
                        cellsList[tempSourceCellId][0] = 1;
                        if(cellsList[tempSourceCellId][1] == 1)
                        {
                            checkedListFromSource[tempSourceCellId] = toCheckListFromSource.back();
                            std::list<islandCell>::iterator it;
                            for (it = toCheckListFromTarget.begin(); it != toCheckListFromTarget.end(); ++it)
                            {
                                if(it->cellId == toCheckListFromSource.back().cellId)
                                {
                                    checkedListFromTarget[tempSourceCellId] = {it->positionX, it->positionY, it->cellId, it->cellScore, it->parentId};
                                }
                            }
                            cellIsInBothLists = true;
                            collisionCellId = tempSourceCellId;
                            i = 8;
                        }
                    }
                }
            }
            if(tempTargetCellId >= 0 && tempTargetCellId < maxCellId)
            {
                if(currentGame.isPassable(tempTargetX,tempTargetY))
                {
                    if(cellsList[tempTargetCellId][1] == 0)
                    {
                        toCheckListFromTarget.push_back({tempTargetX, tempTargetY, tempSourceCellId, cellScore, toCheckListFromTarget.front().cellId});
                        cellsList[tempTargetCellId][1] = 1;
                        if(cellsList[tempTargetCellId][0] == 1)
                        {
                            checkedListFromTarget[tempTargetCellId] = toCheckListFromTarget.back();
                            std::list<islandCell>::iterator it;
                            for (it = toCheckListFromSource.begin(); it != toCheckListFromSource.end(); ++it)
                            {
                                if(it->cellId == toCheckListFromTarget.back().cellId)
                                {
                                    checkedListFromSource[tempTargetCellId] = {it->positionX, it->positionY, it->cellId, it->cellScore, it->parentId};
                                }
                            }
                            cellIsInBothLists = true;
                            collisionCellId = tempTargetCellId;
                            i = 8;
                        }
                    }
                }
            }
        }
        checkedListFromSource[toCheckListFromSource.front().cellId] = toCheckListFromSource.front();
        checkedListFromTarget[toCheckListFromTarget.front().cellId] = toCheckListFromTarget.front();
        toCheckListFromSource.pop_front();
        toCheckListFromTarget.pop_front();
        cellScore += +1;
    }
    return cellIsInBothLists;
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
            this->routeNeedsPath = true;
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
                            this->routeNeedsPath = true;
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
            this->routeNeedsPath = true;
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
    if(this->routeNeedsPath){
        this->noPathPossible = false;
        this->forwardIsDone = false;
        this->collisionCell = -1;
        this->mapArray.reserve(MAP_HEIGHT*MAP_WIDTH);
        this->mapArrayBack.reserve(MAP_HEIGHT*MAP_WIDTH);
        for(int i = 0; i < MAP_HEIGHT*MAP_WIDTH; i++)
        {
            this->mapArray[i] = 0;
            this->mapArrayBack[i] = 0;
        }
        std::thread pathfinding(&actors::pathAStar, &listOfActors[this->actorId], false);
        //std::thread pathfindingBackwards(&actors::pathAStar, &listOfActors[this->actorId], true);
        if(canTargetBeReached())
        {
            pathfinding.join();
            //pathfindingBackwards.join();
        }
        else
        {
            this->noPathPossible = true;
            pathfinding.join();
            //pathfindingBackwards.join();
        }
        this->routeNeedsPath = false;
    }
}


std::mutex mtx;

void actors::pathAStar(bool backward)
{
    std::vector<Cells> cellsList;
    cellsList.reserve(MAP_HEIGHT*MAP_WIDTH);
    updateCells(this->actorGoal[0], this->actorGoal[1], cellsList);
    std::list<Cells> listToCheck;
    std::list<Cells> checkedList;
    int startCell;
    int endCell;
    if(!backward)
    {
        startCell = (actorCords[0]*MAP_HEIGHT)+actorCords[1]; //eigen positie
        endCell = (actorGoal[0]*MAP_HEIGHT)+actorGoal[1]; //doel positie
    }
    else
    {
        endCell = (actorCords[0]*MAP_HEIGHT)+actorCords[1]; //eigen positie
        startCell = (actorGoal[0]*MAP_HEIGHT)+actorGoal[1]; //doel positie
    }
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
        this->pathFound = false;
        addNeighbours(startCell, cellsList);
        cellsList[startCell].visited = true;
        listToCheck.push_back(cellsList[startCell]);
    }

    while(!listToCheck.empty() && startCell != endCell)
    {
        if(this->collisionCell != -1)
        {
            listToCheck.clear();
            this->pathFound = true;
        }
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
        else if(this->noPathPossible)
        {
            listToCheck.clear();
        }
        else
        {
            for(int q = 0; q < 8; q++)
            {
                if(listToCheck.front().neighbours[q] != -1)
                {
                    int newCellId = listToCheck.front().neighbours[q];
                    //We have found neighbours!
                    //check if neighbours was found before
                    if(!cellsList[newCellId].visited)
                    {
                        //Deze cell heeft geen parent is is dus nooit eerder gevonden! De buren moeten dus toegevoegd worden!
                        addNeighbours(newCellId, cellsList);
                        //De cell waarvan we de neighbours onderzoeken is dus automagisch tot nu toe de kortste route hiernaartoe
                        cellsList[newCellId].parentCellId = listToCheck.front().cellId;
                        //Nu moeten de kosten voor de route hiernatoe uitgerekend worden (Dit zijn de kosten van naar de buurman gaan +1
                        cellsList[newCellId].cummulativeCost = listToCheck.front().cummulativeCost+1;
                        //Als laatste zetten we de cell in de lijst met cellen die gecheckt moet worden
                        listToCheck.push_back(cellsList[newCellId]);
                        //Bereken de afstand naar het doel
                        cellsList[newCellId].costToGoal = dist(cellsList[newCellId].positionX,cellsList[newCellId].positionY,cellsList[endCell].positionX,cellsList[endCell].positionY);
                        cellsList[newCellId].totalCostGuess = cellsList[newCellId].costToGoal + cellsList[newCellId].cummulativeCost;
                        cellsList[newCellId].visited = true;
                        while(!mtx.try_lock()){}
                        if(backward)
                        {
                            if(this->mapArray[newCellId] == 1)
                            {
                                //Collisiion!
                                this->collisionCell = newCellId;
                            }
                            else
                            {
                                this->mapArrayBack[newCellId] = 1;

                            }
                        }
                        else
                        {
                            if(this->mapArrayBack[newCellId] == 1)
                            {
                                //Collisiion!
                                this->collisionCell = newCellId;
                            }
                            else
                            {
                                this->mapArray[newCellId] = 1;
                            }
                        }
                        mtx.unlock();
                    }
                    else
                    {
                        //Deze cell is al eerder gevonden, staat dus al in de te checken cell lijst
                        if(listToCheck.front().cummulativeCost+1 < cellsList[newCellId].cummulativeCost)
                        {
                            //Er is een kortere route naar deze cell! Pas de parent cell dus aan en geef een nieuwe cummulative Cost;
                            cellsList[newCellId].parentCellId = listToCheck.front().cellId;
                            cellsList[newCellId].cummulativeCost = listToCheck.front().cummulativeCost+1;
                            cellsList[newCellId].totalCostGuess = cellsList[newCellId].costToGoal + cellsList[newCellId].cummulativeCost;
                        }
                    }
                }
            }
            checkedList.push_back(listToCheck.front());
            listToCheck.pop_front();
        }
    }



    //Zet de te lopen route in een lijst
    if(!backward)
    {
        while(!mtx.try_lock()){}
        this->route.clear();
        if(this->collisionCell == -1)
        {
            this->route.push_back({cellsList[endCell].positionX, cellsList[endCell].positionY, cellsList[endCell].visited, cellsList[endCell].parentCellId});
        }
        else
        {
            this->route.push_back({cellsList[collisionCell].positionX, cellsList[collisionCell].positionY, cellsList[collisionCell].visited, cellsList[collisionCell].parentCellId});
        }
        if(this->pathFound)
        {
            while(!endReached)
            {
                if(route.back().visited == true)
                {

                    this->route.push_back({cellsList[route.back().parentCellId].positionX, cellsList[route.back().parentCellId].positionY, cellsList[route.back().parentCellId].visited, cellsList[route.back().parentCellId].parentCellId});
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
        this->forwardIsDone = true;
        mtx.unlock();
    }
    else if(this->pathFound && this->collisionCell != -1)
    {
        while(!this->forwardIsDone){ }
        while(!mtx.try_lock()){}
        while(!endReached)
        {
            if(route.front().visited == true)
            {
                this->route.push_front({cellsList[route.front().parentCellId].positionX, cellsList[route.front().parentCellId].positionY, cellsList[route.front().parentCellId].visited, cellsList[route.front().parentCellId].parentCellId});
                if(this->route.front().parentCellId == startCell)
                {
                    this->route.push_front({cellsList[route.front().parentCellId].positionX, cellsList[route.front().parentCellId].positionY, cellsList[route.front().parentCellId].visited, cellsList[route.front().parentCellId].parentCellId});
                    endReached = true;
                }
            }
            else
            {
                endReached = true;
            }
        }
        mtx.unlock();
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
    int spriteOffset = 0;
    if(this->isAtRecource)
    {
        if(this->ResourceBeingGatherd == 0)
        {
            spriteOffset = 128;
        }
        else if(this->ResourceBeingGatherd == 2 || this->ResourceBeingGatherd == 3)
        {
            spriteOffset = 256;
        }
        else if(this->ResourceBeingGatherd == 1)
        {
            spriteOffset = 384;
        }
    }
    switch(this->orientation)
    {
    case 0:
        spriteXoffset = 80 + spriteOffset;
        break;
    case 1:
        spriteXoffset = 0 + spriteOffset;
        break;
    case 2:
        spriteXoffset = 96 + spriteOffset;
        break;
    case 3:
        spriteXoffset = 48 + spriteOffset;
        break;
    case 4:
        spriteXoffset = 64 + spriteOffset;
        break;
    case 5:
        spriteXoffset = 32 + spriteOffset;
        break;
    case 6:
        spriteXoffset = 112 + spriteOffset;
        break;
    case 7:
        spriteXoffset = 16 + spriteOffset;
        break;
    default:
        spriteXoffset = 0 + spriteOffset;
        break;
    }
    xPosition = xPosition + this->offSetX;
    yPosition = yPosition + this->offSetY;
    currentGame.spriteVillager.setPosition(xPosition, yPosition);
    currentGame.spriteVillager.setTextureRect(sf::IntRect(spriteXoffset,spriteYoffset,16,32));
    window.draw(currentGame.spriteVillager);
}


void actors::renderPath()
{
    std::list<routeCell>::iterator it;
    for(it = route.begin(); it!=route.end(); it++)
    {
        currentGame.spriteSelectedTileForPath.setPosition(worldSpace(it->positionX, it->positionY, true), worldSpace(it->positionX, it->positionY, false));
        window.draw(currentGame.spriteSelectedTileForPath);
    }
}
