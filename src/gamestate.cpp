#include "gamestate.h"
#include "buildings.h"
#include "objects.h"
#include "actors.h"
#include "player.h"
#include <iostream>
#include <sstream>

void gameState::drawMousePosition(int x,int y, int noProblem)
{
    if(noProblem)
    {
        spriteSelectedTile.setPosition(worldSpace(x, y,true), worldSpace(x, y,false));
        window.draw(spriteSelectedTile);
    }
    else
    {
        spriteTileObstructed.setPosition(worldSpace(x, y,true), worldSpace(x, y,false));
        window.draw(spriteTileObstructed);
    }
}

bool gameState::isPassable(int x, int y)
{
    //check if the terrain is passable 1-6
    if((this->currentMap[x][y] > 0 && this->currentMap[x][y] < 7)&& this->objectLocationList[x][y] == -1 && this->occupiedByBuildingList[x][y] == -1 && this->occupiedByActorList[x][y] == -1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void gameState::drawGround(int i, int j)
{
    switch(currentGame.currentMap[i][j])
    {
    case 0:
        spriteEmptyTile.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteEmptyTile);
        break;
    case 1:
        spriteGrassTile.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteGrassTile);
        break;
    case 2:
        spriteSandTile.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteSandTile);
        break;
    case 3:
        spriteSandTileNE.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteSandTileNE);
        break;
    case 4:
        spriteSandTileNW.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteSandTileNW);
        break;
    case 5:
        spriteSandTileSE.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteSandTileSE);
        break;
    case 6:
        spriteSandTileSW.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteSandTileSW);
        break;
    case 7:
        spriteWaterTile.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteWaterTile);
        break;
    case 8:
        spriteBeachTileNE.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteBeachTileNE);
        break;
    case 9:
        spriteBeachTileNW.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteBeachTileNW);
        break;
    case 10:
        spriteBeachTileSE.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteBeachTileSE);
        break;
    case 11:
        spriteBeachTileSW.setPosition(worldSpace(i,j,true), worldSpace(i,j,false));
        window.draw(spriteBeachTileSW);
        break;
    }
}

void gameState::drawThingsOnTile(int i, int j, bool redraw)
{
    if(!redraw){drawGround(i, j);}
    //check if there are building on the tile and draw them
    if(this->buildingLocationList[i][j] != -1)
    {
        listOfBuildings[this->buildingLocationList[i][j]].drawBuilding(i, j, 0, false);
    }
    else if(this->objectLocationList[i][j]!= -1)
    {
        listOfObjects[this->objectLocationList[i][j]].drawObject(i, j);
    }
    else if(this->occupiedByActorList[i][j] != -1)
    {
        listOfActors[this->occupiedByActorList[i][j]].drawActor();
    }
    else if(this->movedToByActorList[i][j] != -1)
    {
        listOfActors[this->movedToByActorList[i][j]].drawActor();
    }
    else if(this->movedFromByActorList[i][j] != -1)
    {
        listOfActors[this->movedFromByActorList[i][j]].drawActor();
    }
    //check if there are actors on the tile and draw them
}

mouseWorldCord toWorldMousePosition(int mouseX, int mouseY)
{
    if(!(mouseX < 0 ||mouseY < 0))
    {
        auto cheatTile = currentGame.textureCheatTile.copyToImage();
        int cellX = mouseX/64;
        int cellY = mouseY/32;
        int offSetX = mouseX% 64;
        int offSetY = mouseY % 32;
        int worldX = (cellY-mapOffsetY)+(cellX-mapOffsetX);
        int worldY = (cellY-mapOffsetY)-(cellX-mapOffsetX);
        auto color = cheatTile.getPixel(offSetX, offSetY);
        if(color == sf::Color::Red)
        {
            worldX += -1;
        }
        else if(color == sf::Color::Green)
        {
            worldY += +1;
        }
        else if(color == sf::Color::Blue)
        {
            worldY += -1;
        }
        else if(color == sf::Color::Yellow)
        {
            worldX += +1;
        }
        return {worldX, worldY};
    }
    else
    {
        return {0,0};
    }
}

void gameState::drawMap()
{
    mouseWorldCord lowX = toWorldMousePosition(viewOffsetX-(mainWindowWidth/2), viewOffsetY-((mainWindowWidth*0.8)/2));
    mouseWorldCord highX = toWorldMousePosition(viewOffsetX+(mainWindowWidth/2), viewOffsetY+((mainWindowWidth*0.8)/2));
    mouseWorldCord lowY = toWorldMousePosition(viewOffsetX, viewOffsetY-(mainWindowWidth/2));
    mouseWorldCord highY = toWorldMousePosition(viewOffsetX, viewOffsetY+(mainWindowWidth/2));
    for(int j = 0; j < MAP_HEIGHT; j++)
    {
        for(int i = 0; i < MAP_WIDTH; i++ )
        {
            if((i >= lowX.x && i <= highX.x)&&(j >= lowY.y && j <= highY.y))
            {
                drawThingsOnTile(i, j, false);
            }
        }
    }
}

void gameState::loadTextures()
{
    if(textureTileObstructed.loadFromFile("textures/tileObstructed.png"))
    {
        spriteTileObstructed.setTexture(textureTileObstructed);
    }
    else
    {
        std::cout << "Error loading texture: tileObstructed.png \n" << std::endl;
    }
    if(textureBuildingHouse.loadFromFile("textures/house.png"))
    {
        spriteBuildingHouse.setTexture(textureBuildingHouse);
        spriteBuildingHouse.setOrigin(32,96);
    }
    else
    {
        std::cout << "Error loading texture: house.png \n" << std::endl;
    }
    if(texturePineTreeTile.loadFromFile("textures/pineTree.png"))
    {
        spritePineTreeTile.setTexture(texturePineTreeTile);
        spritePineTreeTile.setOrigin(0,32);
    }
    else
    {
        std::cout << "Error loading texture: normalTree.png \n" << std::endl;
    }
    if(textureNormalTreeTile.loadFromFile("textures/normalTree.png"))
    {
        spriteNormalTreeTile.setTexture(textureNormalTreeTile);
        spriteNormalTreeTile.setOrigin(0,32);
    }
    else
    {
        std::cout << "Error loading texture: normalTree.png \n" << std::endl;
    }
    if(textureCypressTrileTile.loadFromFile("textures/cypressTree.png"))
    {
        spriteCypressTrileTile.setTexture(textureCypressTrileTile);
        spriteCypressTrileTile.setOrigin(0,32);
    }
    else
    {
        std::cout << "Error loading texture: cypressTree.png \n" << std::endl;
    }
    if(textureTownCenter.loadFromFile("textures/townCenter.png"))
    {
        spriteTownCenter.setTexture(textureTownCenter);
        spriteTownCenter.setOrigin(96,224);
    }
    else
    {
        std::cout << "Error loading texture: townCenter.png \n" << std::endl;
    }
    if(textureCactusTile.loadFromFile("textures/cactus.png"))
    {
        spriteCactusTile.setTexture(textureCactusTile);
        spriteCactusTile.setOrigin(0,32);
    }
    else
    {
        std::cout << "Error loading texture: cactus.png \n" << std::endl;
    }
    if(textureSelectedTile.loadFromFile("textures/tileSelected.png"))
    {
        spriteSelectedTile.setTexture(textureSelectedTile);
    }
    else
    {
        std::cout << "Error loading texture: tileSelected.png \n" << std::endl;
    }
    if(textureSelectedTileForPath.loadFromFile("textures/tileSelectedForPath.png"))
    {
        spriteSelectedTileForPath.setTexture(textureSelectedTileForPath);
    }
    else
    {
        std::cout << "Error loading texture: tileSelected.png \n" << std::endl;
    }
    if(textureEmptyTile.loadFromFile("textures/emptyTile.png"))
    {
        spriteEmptyTile.setTexture(textureEmptyTile);
    }
    else
    {
        std::cout << "Error loading texture: emptyTile.png \n" << std::endl;
    }
    if(textureGrassTile.loadFromFile("textures/grassTile.png"))
    {
        spriteGrassTile.setTexture(textureGrassTile);
    }
    else
    {
        std::cout << "Error loading texture: grassTile.png \n" << std::endl;
    }
    if(textureSandTile.loadFromFile("textures/sand.png"))
    {
        spriteSandTile.setTexture(textureSandTile);
    }
    else
    {
        std::cout << "Error loading texture: sand.png \n" << std::endl;
    }
    if(textureSandTileNE.loadFromFile("textures/sandNE.png"))
    {
        spriteSandTileNE.setTexture(textureSandTileNE);
    }
    else
    {
        std::cout << "Error loading texture: sandNE.png \n" << std::endl;
    }
    if(textureSandTileNW.loadFromFile("textures/sandNW.png"))
    {
        spriteSandTileNW.setTexture(textureSandTileNW);
    }
    else
    {
        std::cout << "Error loading texture: sandNW.png \n" << std::endl;
    }
    if(textureSandTileSE.loadFromFile("textures/sandSE.png"))
    {
        spriteSandTileSE.setTexture(textureSandTileSE);
    }
    else
    {
        std::cout << "Error loading texture: sandSE.png \n" << std::endl;
    }
    if(textureSandTileSW.loadFromFile("textures/sandSW.png"))
    {
        spriteSandTileSW.setTexture(textureSandTileSW);
    }
    else
    {
        std::cout << "Error loading texture: sandSW.png \n" << std::endl;
    }
    if(textureWaterTile.loadFromFile("textures/waterTile.png"))
    {
        spriteWaterTile.setTexture(textureWaterTile);
    }
    else
    {
        std::cout << "Error loading texture: waterTile.png \n" << std::endl;
    }
    if(textureBeachTileNE.loadFromFile("textures/beachTileNE.png"))
    {
        spriteBeachTileNE.setTexture(textureBeachTileNE);
    }
    else
    {
        std::cout << "Error loading texture: beachTileNE.png \n" << std::endl;
    }
    if(textureBeachTileSW.loadFromFile("textures/beachTileSW.png"))
    {
        spriteBeachTileSW.setTexture(textureBeachTileSW);
    }
    else
    {
        std::cout << "Error loading texture: beachTileSW.png \n" << std::endl;
    }
    if(textureBeachTileNW.loadFromFile("textures/beachTileNW.png"))
    {
        spriteBeachTileNW.setTexture(textureBeachTileNW);
    }
    else
    {
        std::cout << "Error loading texture: beachTileNW.png \n" << std::endl;
    }
    if(textureBeachTileSE.loadFromFile("textures/beachTileSE.png"))
    {
        spriteBeachTileSE.setTexture(textureBeachTileSE);
    }
    else
    {
        std::cout << "Error loading texture: beachTileSE.png \n" << std::endl;
    }
    if(!textureCheatTile.loadFromFile("textures/cheatTile.png"))
    {
        std::cout << "Error loading texture: cheatTile.png \n" << std::endl;
    }
    if(this->textureVillager.loadFromFile("textures/testVillagerSprite.png"))
    {
        this->spriteVillager.setTexture(textureVillager);
        this->spriteVillager.setTextureRect(sf::IntRect(0,0,16,32));
        this->spriteVillager.setOrigin(-24,12);

    }
    else
    {
        std::cout << "Error loading texture: cheatTile.png \n" << std::endl;
    }
}

void gameState::calculateRectangle()
{
    int endLocation[2]= {0,0};
    int highYLocation[2]= {0,0};
    int lowYLocation[2]= {0,0};
    //Endlocation = current world mouse location
    endLocation[0] = this->mouseWorldPosition.x;
    endLocation[1] = this->mouseWorldPosition.y;

    //calculate highY = same screen Y but with start X
    highYLocation[0] = toWorldMousePosition(this->startMouseCords[0], this->mousePosition.y).x;
    highYLocation[1] = toWorldMousePosition(this->startMouseCords[0], this->mousePosition.y).y;

    //calculate lowY = same screen X but with start Y
    lowYLocation[0] = toWorldMousePosition(this->mousePosition.x, this->startMouseCords[1]).x;
    lowYLocation[1] = toWorldMousePosition(this->mousePosition.x, this->startMouseCords[1]).y;

    //Fix for edge-case: mouse Y is < start mouse Y
    int startLocation2[2] = {this->startLocation[0], this->startLocation[1]};

    if(this->mousePosition.y < this->startMouseCords[1])
    {
        int tempStorage[2] = {endLocation[0], endLocation[1]};
        int tempStorage2[2] = {highYLocation[0], highYLocation[1]};
        endLocation[0] = lowYLocation[0];
        endLocation[1] = lowYLocation[1];
        lowYLocation[0] = tempStorage[0];
        lowYLocation[1] = tempStorage[1];
        highYLocation[0] = startLocation2[0];
        highYLocation[1] = startLocation2[1];
        startLocation2[0] = tempStorage2[0];
        startLocation2[1] = tempStorage2[1];
    }

    //Fix for edge-case: mouse X is < start mouse X
    if(this->mousePosition.x < this->startMouseCords[0])
    {
        int tempStorage[2] = {endLocation[0], endLocation[1]};
        int tempStorage2[2] = {startLocation2[0], startLocation2[1]};
        endLocation[0] = highYLocation[0];
        endLocation[1] = highYLocation[1];
        highYLocation[0] = tempStorage[0];
        highYLocation[1] = tempStorage[1];
        startLocation2[0] = lowYLocation[0];
        startLocation2[1] = lowYLocation[1];
        lowYLocation[0] = tempStorage2[0];
        lowYLocation[1] = tempStorage2[1];
    }

    //Start filling the list!
    this->rectangleCords.clear();
    this->rectangleCords.push_back({startLocation2[0],startLocation2[1]});
    this->rectangleCords.push_back({endLocation[0],endLocation[1]});
    this->rectangleCords.push_back({highYLocation[0],highYLocation[1]});
    this->rectangleCords.push_back({lowYLocation[0], lowYLocation[1]});

    //calculate the inbetweens form startLocation to lowYLocation
    int i = startLocation2[0]+1;
    int j = startLocation2[1]-1;
    while(i <= lowYLocation[0] && j >= lowYLocation[1])
    {
        //add new coördanate {i, j} to a list
        if(i != lowYLocation[0] && j != lowYLocation[1])
        {
            this->rectangleCords.push_back({i, j});
        }
        i++;
        j--;
    }

    //calculate the inbetweens from lowYlocation to endLocation
    i = lowYLocation[0]+1;
    j = lowYLocation[1]+1;
    while( i <= endLocation[0] && j <= endLocation[1])
    {
        //add new coördenates {i, j} to a list
        if(i != endLocation[0] && j != endLocation[1])
        {
            this->rectangleCords.push_back({i, j});
        }
        else
        {
            if(this->rectangleCords.back().y+1 != endLocation[1])
            {
                this->rectangleCords.push_back({this->rectangleCords.back().x+1, this->rectangleCords.back().y+1 });
            }
        }

        i++;
        j++;
    }

    //calculate the inbetweens from highYLocation to endLocation
    i = highYLocation[0]+1;
    j = highYLocation[1]-1;
    while(i <= endLocation[0] && j >= endLocation[1])
    {
        //add new coördenates {i, j} to a list
        if(i != endLocation[0] && j != endLocation[1])
        {
            this->rectangleCords.push_back({i, j});

        }
        else
        {
            if(this->rectangleCords.back().y-1 != endLocation[1])
            {
                this->rectangleCords.push_back({this->rectangleCords.back().x+1, this->rectangleCords.back().y-1 });
            }
        }
        i++;
        j--;
    }

    //calculate the inbetweens from startLocation to highYLocation
    i = startLocation2[0]+1;
    j = startLocation2[1]+1;
    while( i <= highYLocation[0] && j <= highYLocation[1])
    {
        //add new coördanates {i, j}to a list
        if( i != highYLocation[0] && j != highYLocation[1])
        {
            this->rectangleCords.push_back({i, j});
        }
        i++;
        j++;
    }

    //sort the list on the Y coördanates
    if(!this->rectangleCords.empty())
    {
        std::sort(this->rectangleCords.begin(),this->rectangleCords.end(), rectCord);
    }

    //add cells on the list between the two cells with the same Y coördanets repeat until list is empty
    if(!this->rectangleCords.empty())
    {
        int stopAt = this->rectangleCords.size();
        for(int i = 0; i < stopAt; i++)
        {
            if(this->rectangleCords[i].y == this->rectangleCords[i+1].y)
            {
                if(this->rectangleCords[i].x < this->rectangleCords[i+1].x)
                {
                    for(int j = this->rectangleCords[i].x+1; j < this->rectangleCords[i+1].x; j++)
                    {
                        this->rectangleCords.push_back({j, this->rectangleCords[i].y});

                    }
                }
                else
                {
                    for(int j = this->rectangleCords[i+1].x+1; j < this->rectangleCords[i].x; j++)
                    {
                        this->rectangleCords.push_back({j, this->rectangleCords[i].y});
                    }
                }
            }
        }
    }

    //Throw out any cells which are out of bounds
    if(!this->rectangleCords.empty())
    {
        for(int i = 0; i< this->rectangleCords.size();)
        {
            if(this->rectangleCords[i].x < 0 || this->rectangleCords[i].x >= MAP_WIDTH || this->rectangleCords[i].y < 0 || this->rectangleCords[i].y >= MAP_HEIGHT)
            {
                //Deze cell is out of bounds gooi hem weg
                this->rectangleCords.erase(this->rectangleCords.begin()+ i);
            }
            else
            {
                i++;
            }
        }
    }

    //Throw out duplicates
    if(!this->rectangleCords.empty())
    {
        std::sort(this->rectangleCords.begin(), this->rectangleCords.end(), compareCord);
        this->rectangleCords.erase(std::unique( this->rectangleCords.begin(), this->rectangleCords.end(), compareCord), this->rectangleCords.end());
    }
}

void gameState::interact()
{
    this->mouseFakePosition = sf::Mouse::getPosition(window);
    this->mousePosition = window.mapPixelToCoords(mouseFakePosition);

    if(this->focus)
    {
        this->mouseWorldPosition = toWorldMousePosition(this->mousePosition.x, this->mousePosition.y);
    }
    while (window.pollEvent(this->event))
    {
        switch(this->event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::LostFocus:
            this->focus = false;
            break;
        case sf::Event::GainedFocus:
            this->focus = true;
            break;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->focus)
    {
        viewOffsetY += -5;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && this->focus)
    {
        viewOffsetY += +5;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && this->focus)
    {
        viewOffsetX += -5;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && this->focus)
    {
        viewOffsetX += +5;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->focus)
    {
        this->isPressedA = true;
        this->rectangleCords.clear();
    }
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->isPressedA = false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::B) && this->focus)
    {
        this->isPressedB = true;
        this->rectangleCords.clear();
    }
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::B))
    {
        this->isPressedB = false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::O) && this->focus)
    {
        this->isPressedO = true;
        this->rectangleCords.clear();
    }
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::B))
    {
        this->isPressedO = false;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab) && this->focus && !this->isPressedTab)
    {
        this->isPressedTab = true;
        int tempTeam = currentPlayer.getTeam()+1;
        if(tempTeam > 7){
            currentPlayer = listOfPlayers[1];
        } else {
            currentPlayer = listOfPlayers[tempTeam];
        }

    }
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
    {
        this->isPressedTab = false;
    }

    //Mouse interaction Left
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->mousePressedLeft && this->focus)
    {
        this->mousePressedLeft = true;
        if(mouseFakePosition.y > mainWindowHeigth*0.8f)
        {
            this->mousePressOutofWorld = true;
            if(mouseFakePosition.x> mainWindowWidth*0.8f)
            {
                int realMouseX = mouseFakePosition.x - (mainWindowWidth*0.8f);
                int realMouseY = mouseFakePosition.y - (mainWindowHeigth*0.8f);
                float newOffsetX = (realMouseX/(0.2f*mainWindowWidth))*(MAP_WIDTH*64);
                float newOffsetY = (realMouseY/(0.2f*mainWindowHeigth))*(MAP_HEIGHT*32);
                viewOffsetX = newOffsetX;
                viewOffsetY = newOffsetY;
            }
        }
        else
        {
            this->mousePressOutofWorld = false;
            if(this->isPressedB)
            {
                if(!(this->mouseWorldPosition.x-footprintOfBuildings[this->buildingTypeSelected].amountOfXFootprint < -1) && !(this->mouseWorldPosition.y-footprintOfBuildings[this->buildingTypeSelected].amountOfYFootprint < -1) && !(this->mouseWorldPosition.x >= MAP_WIDTH) && !(this->mouseWorldPosition.y >= MAP_HEIGHT))
                {
                    //check of het gebouw hier kan staan:
                    bool buildinPlacable = true;
                    for(int i = 0; i < footprintOfBuildings[this->buildingTypeSelected].amountOfXFootprint; i++)
                    {
                        for(int j = 0; j < footprintOfBuildings[this->buildingTypeSelected].amountOfYFootprint; j++)
                        {
                            if(this->occupiedByBuildingList[this->mouseWorldPosition.x-i][this->mouseWorldPosition.y-j] != -1 || this->objectLocationList[mouseWorldPosition.x-i][mouseWorldPosition.y-j] != -1)
                            {
                                buildinPlacable = false;
                            }
                        }
                    }
                    if(buildinPlacable)
                    {
                        //Zet het gebouw neer
                        buildings newBuilding(this->buildingTypeSelected, this->mouseWorldPosition.x, this->mouseWorldPosition.y, listOfBuildings.size(), currentPlayer.getTeam());
                        listOfBuildings.push_back(newBuilding);
                    }
                    else
                    {
                        std::cout << "Can't place building the space is occupied!\n";
                    }
                }
            }
            else if(this->isPressedO)
            {
                if(!(this->mouseWorldPosition.x < 0) && !(this->mouseWorldPosition.y < 0) && !(this->mouseWorldPosition.x >= MAP_WIDTH) && !(this->mouseWorldPosition.y >= MAP_HEIGHT))
                {
                    //check of het object hier kan staan:
                    if(this->objectLocationList[mouseWorldPosition.x][mouseWorldPosition.y] == -1 && this->occupiedByBuildingList[this->mouseWorldPosition.x][this->mouseWorldPosition.y] == -1)
                    {
                        //Zet het object neer
                        objects newObject(this->objectTypeSelected, this->mouseWorldPosition.x, this->mouseWorldPosition.y, listOfObjects.size());
                        listOfObjects.push_back(newObject);
                    }
                    else
                    {
                        std::cout << "Can't place objectt the space is occupied!\n";
                    }
                }

            }
            else if(this->isPressedA)
            {
                if(!(this->mouseWorldPosition.x < 0) && !(this->mouseWorldPosition.y < 0) && !(this->mouseWorldPosition.x >= MAP_WIDTH) && !(this->mouseWorldPosition.y >= MAP_HEIGHT))
                {
                    //check of de actor hier kan staan:
                    if(this->objectLocationList[mouseWorldPosition.x][mouseWorldPosition.y] == -1 && this->occupiedByBuildingList[this->mouseWorldPosition.x][this->mouseWorldPosition.y] == -1 && this->occupiedByActorList[mouseWorldPosition.x][mouseWorldPosition.y] == -1)
                    {
                        //Zet de actor neer
                        actors newActor(1, this->mouseWorldPosition.x, this->mouseWorldPosition.y, currentPlayer.getTeam(),  listOfActors.size());
                        listOfActors.push_back(newActor);
                    }
                    else
                    {
                        std::cout << "Can't place actor the space is occupied!\n";
                    }
                }

            }
            else
            {
                if(!(this->mouseWorldPosition.x >= MAP_WIDTH || this->mouseWorldPosition.x < 0 || this->mouseWorldPosition.y >= MAP_HEIGHT || this->mouseWorldPosition.y < 0))
                {
                    this->startLocation[0] = this->mouseWorldPosition.x;
                    this->startLocation[1] = this->mouseWorldPosition.y;
                    this->startMouseCords[0] = this->mousePosition.x;
                    this->startMouseCords[1] = this->mousePosition.y;
                }
            }
        }
    }
    else if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        this->selectedUnits.clear();
        this->mousePressedLeft = false;
        if(!this->rectangleCords.empty())
        {
            for(int i = 0; i < this->rectangleCords.size(); i++)
            {
                if(this->occupiedByActorList[this->rectangleCords[i].x][this->rectangleCords[i].y] != -1)
                {
                    selectedUnits.push_back({this->occupiedByActorList[this->rectangleCords[i].x][this->rectangleCords[i].y]});
                }
            }
        }
    }
    //Mouse interaction Right
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !this->mousePressedRight && this->focus)
    {
        this->rectangleCords.clear();
        if(this->isPressedB)
        {
            this->buildingTypeSelected += 1;
            if(this->buildingTypeSelected > 1)
            {
                this->buildingTypeSelected = 0;
            }
        }
        else if(this->isPressedO)
        {
            this->objectTypeSelected += 1;
            if(this->objectTypeSelected > 3)
            {
                this->objectTypeSelected = 0;
            }
        }
        else if(!selectedUnits.empty())
        {
            this->firstRound = true;
            this->lastIandJ[0] = 0;
            this->lastIandJ[1] = 0;
            if(this->isPassable(this->mouseWorldPosition.x, this->mouseWorldPosition.y))
            {
                for(int i = 0; i < this->selectedUnits.size(); i++)
                {
                    if(this->selectedUnits.size() > 1)
                    {
                        mouseWorldCord tempCords = currentGame.getNextCord(this->mouseWorldPosition.x, this->mouseWorldPosition.y);
                        listOfActors[this->selectedUnits[i]].updateGoal(tempCords.x, tempCords.y, i/5);
                        listOfActors[this->selectedUnits[i]].setCommonGoalTrue();
                    }
                    else
                    {
                        listOfActors[this->selectedUnits[i]].updateGoal(this->mouseWorldPosition.x, this->mouseWorldPosition.y, 0);
                    }
                    listOfActors[this->selectedUnits[i]].setGatheringRecource(false);
                }
            }
            else if(this->objectLocationList[this->mouseWorldPosition.x][this->mouseWorldPosition.y] != -1)
            {
                for(int i = 0; i < this->selectedUnits.size(); i++)
                {
                    if(this->selectedUnits.size() > 1)
                    {
                        listOfActors[this->selectedUnits[i]].updateGoal(this->mouseWorldPosition.x, this->mouseWorldPosition.y, i/5);
                        listOfActors[this->selectedUnits[i]].setCommonGoalTrue();
                    } else {
                        listOfActors[this->selectedUnits[i]].updateGoal(this->mouseWorldPosition.x, this->mouseWorldPosition.y, 0);
                    }
                    listOfActors[this->selectedUnits[i]].setGatheringRecource(true);
                }
            }
        }
        this->mousePressedRight = true;
    }
    else if(!sf::Mouse::isButtonPressed(sf::Mouse::Right))
    {
        this->mousePressedRight = false;
    }


    if(this->mousePressedLeft && !this->isPressedB && !this->isPressedO && !this->mousePressOutofWorld)
    {
        calculateRectangle();
    }



    if(!this->rectangleCords.empty() && sf::Keyboard::isKeyPressed(sf::Keyboard::Equal) &&!this->equalIsPressed && this->focus)
    {
        for(int i = 0; i < this->rectangleCords.size(); i++)
        {
            this->currentMap[this->rectangleCords[i].x][this->rectangleCords[i].y] += +1;
            if(this->currentMap[this->rectangleCords[i].x][this->rectangleCords[i].y] > 11)
            {
                this->currentMap[this->rectangleCords[i].x][this->rectangleCords[i].y] = 1;
            }
        }
        minimapTextureExist = false;
        this->equalIsPressed = true;
    }
    else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
    {
        this->equalIsPressed = false;
    }
}

void gameState::drawMouseInteraction()
{
    if(!rectangleCords.empty())
    {
        for(int i = 0; i < rectangleCords.size(); i++)
        {
            drawMousePosition(rectangleCords[i].x, rectangleCords[i].y,true);
        }
    }
    if(isPressedB)
    {
        listOfBuildings[0].drawBuildingFootprint(this->buildingTypeSelected, mouseWorldPosition.x, mouseWorldPosition.y);
    }
    if(isPressedO)
    {
        listOfObjects[0].drawObjectFootprint(this->objectTypeSelected, mouseWorldPosition.x, mouseWorldPosition.y);
    }
}

void gameState::update()
{
    if(!listOfActors.empty())
    {
        for(int i = 0; i < listOfActors.size(); i++)
        {
            listOfActors[i].update();
        }
    }
    if(!listOfObjects.empty()){
        for(int i = 0; i < listOfObjects.size(); i++)
        {
            listOfObjects[i].update();
        }
    }

}

mouseWorldCord gameState::getNextCord(int x, int y)
{
    if(!this->firstRound && this->roundDone)
    {
        if(this->lastIandJ[0] == this->lastIandJ[1])
        {
            this->lastIandJ[0] += 1;
            this->lastIandJ[1]  = 0;
        }
        else
        {
            this->lastIandJ[1] += 1;
        }
        this->addSubstractX = true;
        this->addSubstractY = true;

    }
    else
    {
        this->firstRound = false;
    }

    if(this->addSubstractX)
    {
        if(this->addSubstractY)
        {
            x = x + this->lastIandJ[0];
            y = y + this->lastIandJ[1];
            this->addSubstractY = false;
        }
        else
        {
            x = x + this->lastIandJ[0];
            y = y - this->lastIandJ[1] ;
            this->addSubstractX = false;
            this->addSubstractY = true;
        }
    }
    else
    {
        if(this->addSubstractY)
        {
            x = x - this->lastIandJ[0];
            y = y + this->lastIandJ[1];
            this->addSubstractY = false;
        }
        else
        {
            x = x - this->lastIandJ[0];
            y = y - this->lastIandJ[1] ;
            this->roundDone = true;
        }

    }


    if(currentGame.isPassable(x, y))
    {
        return {x, y};
    }
    else
    {
        currentGame.getNextCord(x, y);
    }
}

void gameState::drawMiniMap()
{
    window.setView(miniMap);
    sf::RectangleShape miniMapPixel(sf::Vector2f(20.f,10.f));
    if(!minimapTextureExist)
    {
        minimapTexture.create(20*MAP_WIDTH,10*MAP_HEIGHT);
        minimapTexture.clear(sf::Color(0,0,0,0));
        for(int j = 0; j < MAP_HEIGHT; j++)
        {
            for(int i = 0; i < MAP_WIDTH; i++ )
            {
                switch(currentGame.currentMap[i][j])
                {
                case 0:
                    miniMapPixel.setFillColor(sf::Color(0, 0, 0));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 1:
                    miniMapPixel.setFillColor(sf::Color(152, 205, 115));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 2:
                    miniMapPixel.setFillColor(sf::Color(200, 160, 80));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 3:
                    miniMapPixel.setFillColor(sf::Color(200, 160, 80));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 4:
                    miniMapPixel.setFillColor(sf::Color(200, 160, 80));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 5:
                    miniMapPixel.setFillColor(sf::Color(200, 160, 80));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 6:
                    miniMapPixel.setFillColor(sf::Color(200, 160, 80));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 7:
                    miniMapPixel.setFillColor(sf::Color(69, 164, 208));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 8:
                    miniMapPixel.setFillColor(sf::Color(69, 164, 208));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 9:
                    miniMapPixel.setFillColor(sf::Color(69, 164, 208));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 10:
                    miniMapPixel.setFillColor(sf::Color(69, 164, 208));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                case 11:
                    miniMapPixel.setFillColor(sf::Color(69, 164, 208));
                    miniMapPixel.setPosition(miniMapSpace(i,j,true), miniMapSpace(i,j,false));
                    minimapTexture.draw(miniMapPixel);
                    break;
                }
            }

        }
        minimapTexture.display();
        minimapTextureExist = true;
    }
    sf::Sprite miniMapBackground;
    miniMapBackground.setTexture(minimapTexture.getTexture());
    miniMapBackground.setScale(this->miniMapWidth/(20*MAP_WIDTH), this->miniMapHeigth/(10*MAP_HEIGHT));
    window.draw(miniMapBackground);
    sf::RectangleShape viewBox(sf::Vector2f(this->viewBoxX, this->viewBoxY));
    viewBox.setOrigin(sf::Vector2f(viewBoxX/2.f, viewBoxY/2.f));
    viewBox.setFillColor(sf::Color(0,0,0,0));
    viewBox.setOutlineThickness(3.f);
    viewBox.setOutlineColor(sf::Color(255,255,255));
    float xBoxLocation = (viewOffsetX/(float)this->mapPixelWidth)*this->miniMapWidth;
    float yBoxLocation = (viewOffsetY/(float)this->mapPixelHeigth)*this->miniMapHeigth;
    viewBox.setPosition(xBoxLocation, yBoxLocation);
    window.draw(viewBox);
    window.setView(worldView);
}

void gameState::drawToolbar()
{
    window.setView(toolBar);
    sf::RectangleShape tempBox(sf::Vector2f(mainWindowWidth, this->miniMapHeigth));
    tempBox.setFillColor(sf::Color(116,86,61));
    window.draw(tempBox);
    window.setView(worldView);
}

void gameState::drawTopBar(){
    window.setView(topBar);
    sf::RectangleShape tempBox1(sf::Vector2f(mainWindowWidth, this->topBarHeigth));
    tempBox1.setFillColor(sf::Color(116,86,61));
    window.draw(tempBox1);
    playerStats tempStats = currentPlayer.getStats();
    std::stringstream resourcesText;
    resourcesText   << "Wood: " << tempStats.amountOfWood <<" | Food: " << tempStats.amountOfFood << " | Stone: " << tempStats.amountOfStone << " | Gold: " << tempStats.amountOfGold
                    << " | Population: "<< tempStats.currentPopulation<< "/"<<tempStats.populationRoom << " | Team: " << tempStats.team ;
    text.setString(resourcesText.str());
    text.setCharacterSize(24);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.f);
    text.setFillColor(sf::Color::White);
    window.draw(text);
    window.setView(worldView);
}

void gameState::drawGame()
{
    window.clear(sf::Color(0, 0, 0));
    worldView.setCenter(viewOffsetX, viewOffsetY);;
    window.setView(worldView);
    drawTopBar();
    drawMap();
    drawToolbar();
    drawMiniMap();
    drawMouseInteraction();
    //debugging
    for(int i = 0; i < listOfActors.size(); i++)
    {
        listOfActors[i].renderPath();
    }
    window.display();
}

void gameState::loadMap()
{
    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            this->currentMap[j][i] = 1;
            this->buildingLocationList[j][i] = -1;
            this->occupiedByBuildingList[j][i] = -1;
            this->objectLocationList[j][i] = -1;
            this->occupiedByActorList[j][i] = -1;
            this->movedToByActorList[j][i] = -1;
            this->movedFromByActorList[j][i] = -1;
        }
    }
}

void gameState::loadBuildings()
{
    footprintOfBuildings.push_back({2,2});
    footprintOfBuildings.push_back({4,4});
}

void gameState::loadGame()
{
    worldView.setViewport(sf::FloatRect(0.f, 0.03f, 1.f, 0.77f));
    topBar.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 0.03f));
    toolBar.setViewport(sf::FloatRect(0.f, 0.8f, 1.f, 0.2f));
    miniMap.setViewport(sf::FloatRect(0.8f, 0.8f, 0.2f, 0.2f));
    this->text.setFont(this->font);
    window.setFramerateLimit(60);
    currentGame.loadTextures();
    currentGame.loadMap();
    currentGame.loadBuildings();
    for(int i =0; i < 8; i++){
        listOfPlayers[i].setTeam(i);
    }
    if(!this->font.loadFromFile("fonts/arial.ttf")){std::cout << "Could not load font arial.ttf!\n";}
    this->focus = true;
    this->mousePressedLeft = false;
    this->equalIsPressed = false;
    this->isPressedB = false;
    this->mousePressedRight = false;
    this->mapPixelHeigth = MAP_HEIGHT*32;
    this->mapPixelWidth = MAP_WIDTH*64;
    this->miniMapWidth = mainWindowWidth*0.2f;
    this->miniMapHeigth = mainWindowHeigth*0.2f;
    this->topBarHeigth = mainWindowHeigth*0.03f;
    this->viewBoxX = mainWindowWidth/(this->mapPixelWidth/this->miniMapWidth);
    this->viewBoxY = (mainWindowHeigth*0.77f)/(this->mapPixelHeigth/this->miniMapHeigth);
    this->toolBarWidth = mainWindowWidth-miniMapWidth;

}

