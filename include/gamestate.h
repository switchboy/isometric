#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SFML/Graphics.hpp>
#include "globalfunctions.h"

extern int mainWindowWidth;
extern int mainWindowHeigth;
extern sf::RenderWindow window;
extern sf::View topBar;
extern sf::View worldView;
extern sf::View toolBar;
extern sf::View miniMap;
extern int viewOffsetX;
extern int viewOffsetY;
extern sf::RenderTexture minimapTexture;
extern bool minimapTextureExist;

mouseWorldCord toWorldMousePosition(int mouseX, int mouseY);

class gameState
{

public:
    void drawMousePosition(int x,int y, int noProblem);
    void drawGround(int i, int j);
    void drawThingsOnTile(int i, int j, bool redraw);
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
    mouseWorldCord getNextCord(int x, int y);
    sf::Font font;
    sf::Text text;
    sf::Sprite  spriteSelectedTile,spriteSelectedTileForPath, spriteEmptyTile, spriteGrassTile, spriteSandTile, spriteSandTileNE,
    spriteSandTileNW,spriteSandTileSE,spriteSandTileSW, spriteCactusTile, spriteCypressTrileTile,
    spriteNormalTreeTile, spritePineTreeTile, spriteTileObstructed, spriteTownCenter,
    spriteStone, spriteGold, spriteBerryBush,
    spriteWaterTile, spriteBeachTileNE, spriteBeachTileNW,spriteBeachTileSE,spriteBeachTileSW, spriteBuildingHouse, spriteVillager;

    sf::Texture textureSelectedTile, textureSelectedTileForPath, textureEmptyTile, textureGrassTile, textureSandTile, textureSandTileNE,
    textureSandTileNW,textureSandTileSE,textureSandTileSW, textureCactusTile, textureCypressTrileTile,
    textureNormalTreeTile, texturePineTreeTile, textureCheatTile, textureTileObstructed, textureTownCenter,
    textureStone, textureGold, textureBerryBush,
    textureWaterTile, textureBeachTileNE, textureBeachTileNW,textureBeachTileSE, textureBeachTileSW, textureBuildingHouse, textureVillager;
    int currentMap[MAP_WIDTH][MAP_HEIGHT];
    int buildingLocationList[MAP_WIDTH][MAP_HEIGHT];
    int objectLocationList[MAP_WIDTH][MAP_HEIGHT];
    int occupiedByBuildingList[MAP_WIDTH][MAP_HEIGHT];
    int occupiedByActorList[MAP_WIDTH][MAP_HEIGHT];
    int movedToByActorList[MAP_WIDTH][MAP_HEIGHT];
    int movedFromByActorList[MAP_WIDTH][MAP_HEIGHT];

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
    std::vector<int> selectedUnits;
    std::vector<rectangleCord> rectangleCords;
    sf::Vector2i mouseFakePosition;
    sf::Vector2f mousePosition;


};
extern gameState currentGame;


#endif // GAMESTATE_H
