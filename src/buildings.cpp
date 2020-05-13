#include "buildings.h"
#include "gamestate.h"
#include "player.h"
#include "gametext.h"
#include <iostream>


std::vector<footprintOfBuilding> footprintOfBuildings;
std::vector<actorOrBuildingPrice> priceOfBuilding;
std::vector<buildings> listOfBuildings;


void buildings::fillAdjacentTiles()
{
    //The row above and below the building
    for(int i = this->endXlocation-1; i <= this->startXlocation+1; i++)
    {
        if(i >= 0 && i <= MAP_WIDTH)
        {
            int goalX;
            if(i == this->endXlocation-1)
            {
                goalX = i +1;
            }
            else if(i == this->startXlocation+1)
            {
                goalX = i -1;
            }
            else
            {
                goalX = i;
            }
            adjacentTile newTileAbove = {this->adjacentTiles.size(), i, this->endYLocation-1, goalX, this->endYLocation, false,-1};
            this->adjacentTiles.push_back(newTileAbove);
            adjacentTile newTileBelow = {this->adjacentTiles.size(), i, this->startYLocation+1, goalX, this->startYLocation, false, -1};
            this->adjacentTiles.push_back(newTileBelow);
        }
    }
    //The row left and right of the building
    for(int i =  this->startYLocation-footprintOfBuildings[this->buildingType].amountOfYFootprint; i <=  this->startYLocation; i++)
    {
        if(i >= 0 && i <= MAP_HEIGHT)
        {
            adjacentTile newTileLeft = {this->adjacentTiles.size(), this->endXlocation-1, i, this->endXlocation, i, false, -1 };
            this->adjacentTiles.push_back(newTileLeft);
            adjacentTile newTileRight = {this->adjacentTiles.size(), this->startXlocation+1, i, this->startXlocation, i, false, -1};
            this->adjacentTiles.push_back(newTileRight);
        }
    }
}

void buildings::removeActorFromBuildingTile(int actorId)
{
    for(int i = 0; i < this->adjacentTiles.size(); i++)
    {
        if(this->adjacentTiles[i].actorId == actorId)
        {
            this->adjacentTiles[i].occupied = false;
            this->adjacentTiles[i].actorId = -1;
        }
    }

}

std::vector<adjacentTile> buildings::getFreeBuildingTile()
{
    std::vector<adjacentTile> tileList;
    for(int i = 0; i < this->adjacentTiles.size(); i++)
    {
        if(!this->adjacentTiles[i].occupied)
        {
            if(currentGame.isPassable(this->adjacentTiles[i].tileX, this->adjacentTiles[i].tileY))
            {
                tileList.push_back(this->adjacentTiles[i]);
            }
        }
    }
    return tileList;
}



std::vector<adjacentTile> buildings::getDropOffTiles()
{
    std::vector<adjacentTile> tileList;
    for(int i = 0; i < this->adjacentTiles.size(); i++)
    {
        if(currentGame.isPassable(this->adjacentTiles[i].tileX, this->adjacentTiles[i].tileY))
        {
            tileList.push_back(this->adjacentTiles[i]);
        }
    }
    return tileList;
}


void buildings::claimFreeBuiildingTile(int id, int actorId)
{
    if(this->adjacentTiles[id].actorId == -1)
    {
        this->adjacentTiles[id].actorId = actorId;
        this->adjacentTiles[id].occupied = true;
    }
}

bool buildings::getCompleted()
{
    return this->buildingCompleted;
}

void buildings::setCompleted()
{
    this->buildingCompleted = true;
    gameText.addNewMessage("- Building completed! -", 0);
    listOfPlayers[this->ownedByPlayer].addToPopulationRoom(this->supportsPopulationOf);
}

void buildings::removeBuilding()
{
    this->exists = false;
    currentGame.buildingLocationList[this->startXlocation][this->startYLocation] = -1;
    for(int i = 0; i < footprintOfBuildings[this->buildingType].amountOfXFootprint; i++)
    {
        for(int j = 0; j < footprintOfBuildings[this->buildingType].amountOfYFootprint; j++)
        {
            currentGame.occupiedByBuildingList[startXlocation-i][startYLocation-j] = -1;
        }
    }
    if(currentGame.buildingSelectedId == this->buildingId)
    {
        currentGame.buildingSelectedId = -1;
    }
    noNewBuildings = false;
}
buildings::buildings(int type, int startXlocation, int startYLocation, int buildingId, int team)
{
    this->buildingType = type;
    this->startXlocation = startXlocation;
    this->startYLocation = startYLocation;
    this->endXlocation = this->startXlocation-footprintOfBuildings[this->buildingType].amountOfXFootprint+1;
    this->endYLocation = this->startYLocation-footprintOfBuildings[this->buildingType].amountOfYFootprint+1;
    this->buildingId = buildingId;
    this->ownedByPlayer = team;
    this->buildingCompleted = false;
    this->exists = true;
    currentGame.buildingLocationList[startXlocation][startYLocation] = buildingId;
    for(int i = 0; i < footprintOfBuildings[type].amountOfXFootprint; i++)
    {
        for(int j = 0; j < footprintOfBuildings[type].amountOfYFootprint; j++)
        {
            currentGame.occupiedByBuildingList[startXlocation-i][startYLocation-j] = buildingId;
        }
    }
    noNewBuildings = false;
    switch(type)
    {
    case 0:
        hitPointsTotal = 450;
        hitPointsLeft = 450;
        canDoRangedDamage = false;
        amountOfRangedDamage = 0;
        recievesWood = false;
        recievesStone= false;
        recievesGold = false;
        recievesFood = false;
        buildingPointsNeeded = 25;
        buildingPointsRecieved = 0;
        supportsPopulationOf = 5;
        this->offSetYStore = 0;
        this->amountOfAnimationSprites = 0;
        break;
    case 1:
        hitPointsTotal = 5000;
        hitPointsLeft = 5000;
        canDoRangedDamage = true;
        amountOfRangedDamage = 10;
        recievesWood = true;
        recievesStone= true;
        recievesGold = true;
        recievesFood = true;
        buildingPointsNeeded = 150;
        buildingPointsRecieved = 0;
        supportsPopulationOf = 10;
        this->offSetYStore = 0;
        this->amountOfAnimationSprites = 0;
        break;
    }
    fillAdjacentTiles();
}

int buildings::getTeam()
{
    return this->ownedByPlayer;
}

int buildings::getType()
{
    return this->buildingType;
}

int buildings::getLocationX()
{
    return this->startXlocation;
}

int buildings::getLocationY()
{
    return this->startYLocation;
}

int buildings::getBuildingId()
{
    return this->buildingId;
}

int buildings::getRecievesWhichResources()
{
    if(recievesFood && recievesGold && recievesStone && recievesWood)
    {
        return 4;
    }
    else if(recievesGold)
    {
        return 3;
    }
    else if(recievesStone)
    {
        return 2;
    }
    else if(recievesFood)
    {
        return 1;
    }
    else if(recievesWood)
    {
        return 0;
    }
    else
    {
        return 5;
    }
}

void buildings::addBuildingPoint()
{
    this->buildingPointsRecieved += 1;
}

void buildings::drawBuilding(int i, int j, int type, bool typeOverride)
{
    int transparant;
    int offsetY;
    if(!typeOverride)
    {
        type = this->buildingType;
        if(this->buildingCompleted)
        {
            offsetY = 1;
            if(this->amountOfAnimationSprites > 0)
            {
                offsetY += this->offSetYStore;
                if(offsetY > amountOfAnimationSprites+1)
                {
                    offsetY = 1;
                }
                this->offSetYStore = offsetY;
            }
        }
        else
        {
            offsetY = 0;
        }
        transparant = 255;
    }
    else
    {
        offsetY = 1;
        transparant = 128;
    }
    switch(type)
    {
    case 0:
        currentGame.spriteBuildingHouse.setTextureRect(sf::IntRect(0, currentGame.spriteBuildingHouse.getTextureRect().height*offsetY, currentGame.spriteBuildingHouse.getTextureRect().width, currentGame.spriteBuildingHouse.getTextureRect().height));
        currentGame.spriteBuildingHouse.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        currentGame.spriteBuildingHouse.setColor(sf::Color(255, 255, 255, transparant));
        window.draw(currentGame.spriteBuildingHouse);
        break;
    case 1:
        currentGame.spriteTownCenter.setTextureRect(sf::IntRect(0, currentGame.spriteTownCenter.getTextureRect().height*offsetY, currentGame.spriteTownCenter.getTextureRect().width, currentGame.spriteTownCenter.getTextureRect().height));
        currentGame.spriteTownCenter.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        currentGame.spriteTownCenter.setColor(sf::Color(255, 255, 255, transparant));
        window.draw(currentGame.spriteTownCenter);
        break;
    }
    //Redraw possible overdrawn sprites
    if(!typeOverride)
    {
        for(int k = 1; k < 4; k++)
        {
            for(int y = 1; y < footprintOfBuildings[this->buildingType].amountOfYFootprint; y++)
            {
                currentGame.drawThingsOnTile(i+k, j-y);
            }
        }
    }
}

std::string buildings::getName()
{
    switch(this->buildingType)
    {
    case 0:
        return "House";
    case 1:
        return "Town Center";
    }
}

std::pair<int, int> buildings::getHealth()
{
    return {this->hitPointsLeft, this->hitPointsTotal};
}

std::pair<int, int> buildings::getBuildingPoints()
{
    return {this->buildingPointsRecieved, this->buildingPointsNeeded};
}


int buildings::getRangedDMG()
{
    return this->amountOfRangedDamage;
}

void buildings::drawBuildingFootprint(int type, int mouseWorldX, int mouseWorldY)
{
    if(!(mouseWorldX-footprintOfBuildings[0].amountOfXFootprint < -1) && !(mouseWorldY-footprintOfBuildings[0].amountOfYFootprint < -1) && !(mouseWorldX >= MAP_WIDTH) && !(mouseWorldY >= MAP_HEIGHT))
    {
        drawBuilding(mouseWorldX, mouseWorldY, type, true);
        for(int i = 0; i < footprintOfBuildings[type].amountOfXFootprint; i++)
        {
            for(int j = 0; j < footprintOfBuildings[type].amountOfYFootprint; j++)
            {
                if(currentGame.occupiedByBuildingList[mouseWorldX-i][mouseWorldY-j] == -1)
                {
                    currentGame.drawMousePosition(mouseWorldX-i, mouseWorldY-j, true);
                }
                else
                {
                    currentGame.drawMousePosition(mouseWorldX-i, mouseWorldY-j, false);
                }
            }
        }
    }
}

void  buildings::getTask(bool isResearch, int idOfUnitOrResearch, int productionPointsNeeded)
{
    if(this->productionQueue.size() < 5 )
    {
        this->productionQueue.push_back({isResearch, idOfUnitOrResearch, 0, productionPointsNeeded, 0});
        if(!isResearch){
            currentPlayer.substractResources( 1, priceOfActor[idOfUnitOrResearch].food);
            currentPlayer.substractResources( 0, priceOfActor[idOfUnitOrResearch].wood);
            currentPlayer.substractResources( 2, priceOfActor[idOfUnitOrResearch].stone);
            currentPlayer.substractResources( 3, priceOfActor[idOfUnitOrResearch].gold);
        }
    }
    else
    {
        gameText.addNewMessage("No room in building queue for production...", 1);
    }
}

bool buildings::hasTask()
{
    if(this->productionQueue.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

struct worldCords
{
    int x;
    int y;
};



void addNeighboursOfImpassableNeighbours(int& i, std::vector<Cells> &cellsList, std::list<Cells*>& listToCheck)
{
    if(cellsList[i].positionX > 0)
    {
        cellsList[i-MAP_HEIGHT].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i-MAP_HEIGHT]);
    }
    if(cellsList[i].positionX < MAP_WIDTH-1)
    {
        cellsList[i+MAP_HEIGHT].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i+MAP_HEIGHT]);
    }
    if(cellsList[i].positionY > 0)
    {
        cellsList[i-1].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i-1]);
    }
    if(cellsList[i].positionY != MAP_HEIGHT-1)
    {
        cellsList[i+1].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i+1]);
    }
    if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX < MAP_WIDTH-1)
    {
        cellsList[i+1+MAP_HEIGHT].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i+1+MAP_HEIGHT]);
    }
    if(cellsList[i].positionY >0 && cellsList[i].positionX < MAP_WIDTH-1)
    {
        cellsList[i-1+MAP_HEIGHT].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i-1+MAP_HEIGHT]);
    }
    if(cellsList[i].positionY != MAP_HEIGHT-1 && cellsList[i].positionX > 0)
    {
        cellsList[i+1-MAP_HEIGHT].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i+1-MAP_HEIGHT]);
    }
    if(cellsList[i].positionY >0 && cellsList[i].positionX > 0)
    {
        cellsList[i-1-MAP_HEIGHT].addNeighbours(cellsList);
        listToCheck.push_back(&cellsList[i-1-MAP_HEIGHT]);
    }
}



worldCords findEmptySpot(worldCords startCords)
{
    if(currentGame.isPassable(startCords.x, startCords.y))
    {
        return startCords;
    }
    else
    {
        std::vector<Cells> cellsList;
        cellsList.reserve(MAP_HEIGHT*MAP_WIDTH);
        int startCell = (startCords.x*MAP_HEIGHT)+startCords.y;
        updateCells(-1, -1, cellsList);
        std::list<Cells*> listToCheck;
        cellsList[startCell].addNeighbours(cellsList);
        cellsList[startCell].visited = true;
        listToCheck.push_back(&cellsList[startCell]);
        bool freeCellFound = false;
        while(!freeCellFound && !listToCheck.empty())
        {
            for (std::vector<int>::const_iterator iterator =  (*listToCheck.front()).neighbours.begin(), end =  (*listToCheck.front()).neighbours.end(); iterator != end; ++iterator)
            {
                std::cout << cellsList[*iterator].cellId<< " " << cellsList[startCell].cellId << "\n";
                freeCellFound = true;
                return {cellsList[*iterator].positionX, cellsList[*iterator].positionY};
            }
            if(!freeCellFound)
            {
                std::cout << (*listToCheck.front()).cellId << "\n";
                addNeighboursOfImpassableNeighbours((*listToCheck.front()).cellId, cellsList, listToCheck);
            }
            listToCheck.pop_front();
        }
    }
}

void buildings::update()
{
    if(this->exists)
    {
        if(this->buildingCompleted)
        {
            if(!this->productionQueue.empty())
            {
                if(this->productionQueue.front().lastTimeUpdate + 1 < currentGame.elapsedTime)
                {
                    if(this->productionQueue.front().productionPointsGained >= this->productionQueue.front().productionPointsNeeded)
                    {
                        worldCords spawmCords = findEmptySpot({this->startXlocation+1, this->startYLocation+1});
                        if(!this->productionQueue.front().isResearch)
                        {
                            if(currentPlayer.getStats().currentPopulation < currentPlayer.getStats().populationRoom)
                            {
                                switch(this->productionQueue.front().idOfUnitOrResearch)
                                {
                                case 0:
                                    actors newActor(0, spawmCords.x, spawmCords.y, this->ownedByPlayer,  listOfActors.size());
                                    listOfActors.push_back(newActor);
                                    break;
                                }
                                this->productionQueue.erase(productionQueue.begin());
                            }
                            else
                            {
                                if(!this->hasDisplayedError)
                                {
                                    gameText.addNewMessage("Not enough population room to add more units, build more houses!", 1);
                                    this->hasDisplayedError = true;
                                }
                            }

                        }
                        else
                        {
                            //research things do ehh TBI
                            this->productionQueue.erase(productionQueue.begin());
                        }
                    }
                    else
                    {
                        this->productionQueue.front().productionPointsGained += 1;
                        this->productionQueue.front().lastTimeUpdate = currentGame.elapsedTime;
                        this->hasDisplayedError = false;
                    }
                }
            }
        }
        else if(this->buildingPointsNeeded <= this->buildingPointsRecieved)
        {
            this->setCompleted();
        }
    }
}

