#include "button.h"
#include "gamestate.h"
#include <iostream>
#include <sstream>
#include "player.h"
#include "gametext.h"

std::list<button> listOfButtons;

button::button(int positionX, int positionY, int spriteId, int actionType, int actorOrBuildingId, int buttonId)
{
    this->positionX = positionX;
    this->positionY = positionY;
    this->realPositionX = positionX;
    this->realPositionY = positionY+(mainWindowHeigth*0.8f);
    this->spriteId = spriteId;
    this->actionType = actionType;
    this->actorOrBuildingId = actorOrBuildingId;
    this->buttonId = buttonId;
}

bool button::isClicked(sf::Vector2i mousePosition)
{

    sf::IntRect rect(this->realPositionX, this->realPositionY, 64, 64);
    if (rect.contains(mousePosition))
    {
        performAction();
        return true;
    }
    return false;

}

void button::performAction()
{
    switch(this->actionType)
    {
    case 0:
        if(priceOfBuilding[1].food <= currentPlayer.getStats().amountOfFood && priceOfBuilding[1].wood <= currentPlayer.getStats().amountOfWood && priceOfBuilding[1].stone <= currentPlayer.getStats().amountOfStone && priceOfBuilding[1].gold <= currentPlayer.getStats().amountOfGold)
        {
            currentGame.setBuildingType(1);
            currentGame.setIsPlacingBuilding();
        }
        else
        {
            std::stringstream errortext;
            errortext << "Not enough resources to build a town center! Cost Food: "<< priceOfBuilding[1].food <<" Wood: "<< priceOfBuilding[1].wood <<" Stone: "<< priceOfBuilding[1].stone <<" Gold: "<< priceOfBuilding[1].gold;
            gameText.addNewMessage(errortext.str(), 1);
        }
        break;
    case 1:
        if(priceOfBuilding[0].food <= currentPlayer.getStats().amountOfFood && priceOfBuilding[0].wood <= currentPlayer.getStats().amountOfWood && priceOfBuilding[0].stone <= currentPlayer.getStats().amountOfStone && priceOfBuilding[0].gold <= currentPlayer.getStats().amountOfGold)
        {
            currentGame.setBuildingType(0);
            currentGame.setIsPlacingBuilding();
        }
        else
        {
            std::stringstream errortext;
            errortext << "Not enough resources to build a house! Cost Food: "<< priceOfBuilding[0].food <<" Wood: "<< priceOfBuilding[0].wood <<" Stone: "<< priceOfBuilding[0].stone <<" Gold: "<< priceOfBuilding[0].gold;
            gameText.addNewMessage(errortext.str(), 1);

        }
        break;
    case 2:
        //select that unit
        currentGame.selectUnit(this->actorOrBuildingId);
        break;
    }
}


void button::drawButton()
{
    int xOffSet = 0;
    int yOffset = 0;
    switch(this->spriteId)
    {
    case 0:
        xOffSet = 0;
        yOffset = 0;
        break;
    case 1:
        xOffSet = 0;
        yOffset = 64;
        break;
    case 2:
        xOffSet = 64;
        yOffset = 0;
        break;

    }
    currentGame.spriteUIButton.setTextureRect(sf::IntRect(xOffSet, yOffset, 64, 64));
    currentGame.spriteUIButton.setPosition(this->positionX, this->positionY);
    window.draw(currentGame.spriteUIButton);
}

button::~button()
{
    //dtor
}
