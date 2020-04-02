#ifndef BUILDINGS_H
#define BUILDINGS_H
#include <vector>

struct footprintOfBuilding
{
    int amountOfXFootprint;
    int amountOfYFootprint;
};

extern std::vector<footprintOfBuilding> footprintOfBuildings;

class buildings
{

public:
    buildings(int type, int startXlocation, int startYLocation, int buildingId, int team);
    void    createBuilding(int type);
    void    takeDamge();
    void    updateBuilding();
    void    drawBuildingFootprint(int type, int mouseWorldX, int mouseWorldY);
    void    drawBuilding(int i, int j, int type, bool typeOverride);
    int     getRecievesWhichResources();
    int     getType();
    int     getLocationX();
    int     getLocationY();
    int     getBuildingId();
    int     getTeam();

private:
    int     buildingId;
    int     buildingType;
    int     ownedByPlayer;
    int     hitPointsTotal;
    int     hitPointsLeft;
    bool    buildingCompleted;
    int     buildingStage;
    int     buildingPointsNeeded;
    int     buildingPointsRecieved;
    int     startXlocation;
    int     endXlocation;
    int     startYLocation;
    int     endYLocation;
    bool    canDoRangedDamage;
    int     amountOfRangedDamage;
    bool    recievesWood;
    bool    recievesStone;
    bool    recievesGold;
    bool    recievesFood;
    int     supportsPopulationOf;

};
extern std::vector<buildings> listOfBuildings;

#endif // BUILDINGS_H
