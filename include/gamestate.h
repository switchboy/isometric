#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SFML/Graphics.hpp>
#include "globalfunctions.h"
#include "button.h"
#include <list>
#include <sstream>

extern int mainWindowWidth;
extern int mainWindowHeigth;
extern sf::RenderWindow window;
extern sf::View totalView;
extern sf::View topBar;
extern sf::View worldView;
extern sf::View toolBar;
extern sf::View miniMap;
extern int viewOffsetX;
extern int viewOffsetY;
extern sf::RenderTexture minimapTexture;
extern sf::RenderTexture minimapBuildingsTexture;
extern sf::RenderTexture minimapActorsTexture;
extern sf::RenderTexture minimapObjectsTexture;
extern bool noNewBuildings;
extern bool minimapTextureExist;
extern std::list<button> listOfButtons;


struct actorOrBuildingPrice
{
    int food;
    int wood;
    int stone;
    int gold;
};

extern std::vector<actorOrBuildingPrice> priceOfBuilding;
extern std::vector<actorOrBuildingPrice> priceOfActor;

mouseWorldCord toWorldMousePosition(int mouseX, int mouseY);

class gameState
{

public:
    void drawMousePosition(int x,int y, int noProblem);
    void drawGround(int i, int j);
    void drawThingsOnTile(int i, int j);
    void drawMap();
    void loadTextures();
    void calculateRectangle();
    void interact();
    void drawMouseInteraction();
    void drawGame();
    void drawToolbar();
    void loadMap();
    void loadBuildings();
    void loadGame();
    void drawMiniMap();
    void update();
    void drawTopBar();
    float elapsedTime;
    bool isPassable(int x, int y);
    void drawViewBox();
    bool isInSelectedActors(int id);
    void setBuildingType(int id);
    void setIsPlacingBuilding();
    void selectUnit(int id);
    float getTime();
    int buildingSelectedId;
    int objectSelectedId;
    mouseWorldCord getNextCord(int x, int y);
    sf::Font font;
    sf::Text text;
    sf::Sprite  spriteSelectedTile,spriteSelectedTileForPath, spriteEmptyTile, spriteGrassTile, spriteSandTile, spriteSandTileNE,
    spriteSandTileNW,spriteSandTileSE,spriteSandTileSW, spriteCactusTile, spriteCypressTrileTile, spriteBigSelectedIcon,
    spriteNormalTreeTile, spritePineTreeTile, spriteTileObstructed, spriteTownCenter,
    spriteStone, spriteGold, spriteBerryBush, spriteUIButton, spriteUnitSelectedTile,
    spriteWaterTile, spriteBeachTileNE, spriteBeachTileNW,spriteBeachTileSE,spriteBeachTileSW, spriteBuildingHouse, spriteVillager, spriteTotalBackground;

    sf::Texture textureSelectedTile, textureSelectedTileForPath, textureEmptyTile, textureGrassTile, textureSandTile, textureSandTileNE,
    textureSandTileNW,textureSandTileSE,textureSandTileSW, textureCactusTile, textureCypressTrileTile, textureBigSelectedIcon,
    textureNormalTreeTile, texturePineTreeTile, textureCheatTile, textureTileObstructed, textureTownCenter,
    textureStone, textureGold, textureBerryBush, textureUIButton, textureUnitSelectedTile,
    textureWaterTile, textureBeachTileNE, textureBeachTileNW,textureBeachTileSE, textureBeachTileSW, textureBuildingHouse, textureVillager, textureTotalBackground;
    int currentMap[MAP_WIDTH][MAP_HEIGHT];
    int buildingLocationList[MAP_WIDTH][MAP_HEIGHT];
    int objectLocationList[MAP_WIDTH][MAP_HEIGHT];
    int occupiedByBuildingList[MAP_WIDTH][MAP_HEIGHT];
    int occupiedByActorList[MAP_WIDTH][MAP_HEIGHT];


private:
    sf::Event event;
    bool roundDone;
    bool addSubstractX;
    bool addSubstractY;
    bool firstRound;
    int lastIandJ[2];
    float topBarHeigth;
    bool focus;
    bool mousePressedLeft;
    bool equalIsPressed;
    bool mousePressedRight;
    bool isPressedB;
    bool isPressedO;
    bool isPressedA;
    bool isPressedTab;
    bool isPlacingBuilding;
    bool mousePressOutofWorld;
    int buildingTypeSelected;
    int objectTypeSelected;
    int startMouseCords[2];
    int startLocation[2];
    float viewBoxX;
    float viewBoxY;
    int mapPixelWidth;
    int mapPixelHeigth;
    float miniMapWidth;
    float miniMapHeigth;
    int toolBarWidth;
    mouseWorldCord mouseWorldPosition;
    std::vector<rectangleCord> rectangleCords;
    sf::Vector2i mouseFakePosition;
    sf::Vector2f mousePosition;
    std::vector<int> selectedUnits;


};
extern gameState currentGame;


#endif // GAMESTATE_H
