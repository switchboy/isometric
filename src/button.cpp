#include "button.h"
#include "gamestate.h"
#include <iostream>
#include <sstream>
#include "player.h"
#include "gametext.h"
#include "actors.h"
#include "buildings.h"

std::list<button> listOfButtons;

button::button(int positionX, int positionY, int spriteId, int actionType, int actorOrBuildingId, int buttonId, int taskId)
{
    this->positionX = positionX;
    this->positionY = positionY;
    this->realPositionX = positionX;
    this->realPositionY = positionY+(mainWindowHeigth*0.8f);
    this->spriteId = spriteId;
    this->actionType = actionType;
    this->actorOrBuildingId = actorOrBuildingId;
    this->buttonId = buttonId;
    this->taskId = taskId;
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

bool button::isHoverd(sf::Vector2i mousePosition)
{
    sf::IntRect rect(this->realPositionX, this->realPositionY, 64, 64);
    if (rect.contains(mousePosition))
    {
        showToolTip();
        return true;
    }
    return false;
}

void button::showToolTip()
{
    std::stringstream toolTipTitle;
    std::stringstream toolTipText;
    std::stringstream toolTipDiscription;
    switch(this->actionType)
    {
    case 0:
        toolTipTitle << "Town center";
        toolTipText << "Cost: Food: "<< priceOfBuilding[1].food <<" Wood: "<< priceOfBuilding[1].wood <<" Stone: "<< priceOfBuilding[1].stone <<" Gold: "<< priceOfBuilding[1].gold;
        toolTipDiscription << "Collection point of resources, production of new villagers and research facility. Grants 10 population.";
        break;
    case 1:
        toolTipTitle << "House";
        toolTipText << "Cost: Food: "<< priceOfBuilding[0].food <<" Wood: "<< priceOfBuilding[0].wood <<" Stone: "<< priceOfBuilding[0].stone <<" Gold: "<< priceOfBuilding[0].gold;
        toolTipDiscription << "Grants 5 population.";
        break;
    case 2:
        toolTipTitle << "Unit selection";
        toolTipText << listOfActors[this->actorOrBuildingId].nameOfActor();
        toolTipDiscription << "Selects this single unit from the current selection.";
        break;
    case 3:
        toolTipTitle << "Make villager";
        toolTipText << "Cost: Food: "<< priceOfActor[0].food <<" Wood: "<< priceOfActor[0].wood <<" Stone: "<< priceOfActor[0].stone <<" Gold: "<< priceOfActor[0].gold;
        toolTipDiscription << "The town center will start to well... generate a new villager. Villagers can collect resources and build buildings.";
        break;
    case 4:
        toolTipTitle << "Cancel building";
        toolTipText << "Cancels building the current building.";
        toolTipDiscription << "Removes blueprint and returns half of the invested resources.";
        break;
    case 5:
        toolTipTitle << "Cancel production";
        toolTipText << "Cancels research or unit production";
        toolTipDiscription << "Production ceases and half of the invested resources are returned.";
        break;
    }

    int longestStringLength = 0;
    if(toolTipDiscription.str().length() >= toolTipText.str().length() && toolTipDiscription.str().length() >= toolTipTitle.str().length())
    {
        longestStringLength = toolTipDiscription.str().length();
    }
    else if(toolTipText.str().length() >= toolTipDiscription.str().length() && toolTipText.str().length() >= toolTipTitle.str().length())
    {
        longestStringLength = toolTipText.str().length();
    }
    else
    {
        longestStringLength = toolTipTitle.str().length();
    }
    int charSize = 20;
    int startPositionY = (mainWindowHeigth*0.8)-(3*(charSize+2));
    sf::RectangleShape toolTipBox(sf::Vector2f((longestStringLength*8), (3*(charSize+2))+5));
    toolTipBox.setFillColor(sf::Color(0, 0, 0, 75));
    toolTipBox.setOutlineThickness(2.f);
    toolTipBox.setOutlineColor(sf::Color(255, 255, 255,150));
    toolTipBox.setPosition(2, startPositionY-7);
    window.draw(toolTipBox);
    startPositionY -= 4;
    currentGame.text.setCharacterSize(charSize);
    currentGame.text.setOutlineColor(sf::Color::Black);
    currentGame.text.setOutlineThickness(1.f);
    currentGame.text.setFillColor(sf::Color::White);
    currentGame.text.setPosition(5, startPositionY);
    currentGame.text.setString(toolTipTitle.str());
    window.draw(currentGame.text);
    charSize = 16;
    startPositionY += 22;
    currentGame.text.setCharacterSize(charSize);
    currentGame.text.setPosition(5, startPositionY);
    currentGame.text.setString(toolTipText.str());
    window.draw(currentGame.text);
    startPositionY += 18;
    currentGame.text.setCharacterSize(14);
    currentGame.text.setCharacterSize(charSize);
    currentGame.text.setPosition(5, startPositionY);
    currentGame.text.setString(toolTipDiscription.str());
    window.draw(currentGame.text);
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
    case 3:
        //create villager
        if(priceOfActor[0].food <= currentPlayer.getStats().amountOfFood && priceOfActor[0].wood <= currentPlayer.getStats().amountOfWood && priceOfActor[0].stone <= currentPlayer.getStats().amountOfStone && priceOfActor[0].gold <= currentPlayer.getStats().amountOfGold)
        {
            listOfBuildings[this->actorOrBuildingId].getTask(false, 0, 25);
        }
        else
        {
            std::stringstream errortext;
            errortext << "Not enough resources to produce a villager! Cost Food: "<< priceOfActor[0].food <<" Wood: "<< priceOfActor[0].wood <<" Stone: "<< priceOfActor[0].stone <<" Gold: "<< priceOfActor[0].gold;
            gameText.addNewMessage(errortext.str(), 1);
        }
        break;
    case 4:
        //cancel building
        currentPlayer.addResources(1, priceOfBuilding[listOfBuildings[this->actorOrBuildingId].getType()].food/2 );
        currentPlayer.addResources(0, priceOfBuilding[listOfBuildings[this->actorOrBuildingId].getType()].wood/2 );
        currentPlayer.addResources(2, priceOfBuilding[listOfBuildings[this->actorOrBuildingId].getType()].stone/2 );
        currentPlayer.addResources(3, priceOfBuilding[listOfBuildings[this->actorOrBuildingId].getType()].gold/2 );
        listOfBuildings[this->actorOrBuildingId].removeBuilding();
        break;
    case 5:
        //cancel production or research
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
    case 3:
        xOffSet = 192;
        yOffset = 0;

    }
    currentGame.spriteUIButton.setTextureRect(sf::IntRect(xOffSet, yOffset, 64, 64));
    currentGame.spriteUIButton.setPosition(this->positionX, this->positionY);
    window.draw(currentGame.spriteUIButton);
}

button::~button()
{
    //dtor
}
