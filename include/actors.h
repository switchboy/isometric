#ifndef ACTORS_H
#define ACTORS_H
#include <cmath>
#include <list>
#include "gamestate.h"


class Cells{
    public:
        int positionX, positionY, parentCellId, cummulativeCost, cellId;
        double costToGoal, totalCostGuess;
        bool visited = false;
        bool obstacle = false;
        std::list<int> neighbours;
        void updateCells(int goalX, int goalY);
};

extern Cells cellsList[MAP_HEIGHT*MAP_WIDTH];

struct nearestBuildingTile
{
    float deltaDistance;
    int locationX;
    int locationY;
    int buildingId;
    bool isSet;
};

class actors
{
    public:
        actors(int type, int actorX, int actorY, int actorTeam, int actorId);
        virtual ~actors();
        void update();
        void calculateRoute();
        void takeDamage(int amountOfDamage);
        void drawActor();
        void updateGoal(int i, int j, int waitTime);
        void renderPath();
        void setCommonGoalTrue();
        void setGatheringRecource(bool flag);
        nearestBuildingTile findNearestDropOffPoint(int Resource);

    private:
        int actorType;
        int actorTeam;
        int actorHealth;
        bool actorAlive;
        int actorId;
        int hitPoints;
        int totalHitPoints;
        int meleeDamage;
        bool doesRangedDamage;
        int rangedDamage;
        int range;
        int actorCords[2];
        int actorNewCords[2];
        int actorGoal[2];
        int gatheringResourcesAt[2];
        bool busyWalking;
        bool movedMoreThanHalf;
        bool commonGoal;
        bool isGatheringRecources;
        bool isAtRecource;
        bool isAtCarryCapacity;
        bool hasToUnloadResource;
        bool isWalkingToUnloadingPoint;
        bool reachedUnloadingPoint;
        bool hasUnloaded;
        int ResourceBeingGatherd;
        bool carriesRecources;
        int amountOfGold;
        int amountOfWood;
        int amountOfStone;
        int amountOfFood;
        bool pathFound;
        int orientation;
        int currentFrame;
        int spriteYOffset;
        float timeStartedGatheringRecource;
        float timeLastOffsetChange;
        float timeLastUpdate;
        float timeStartedWalkingToRecource;
        int retries;
        float timeLastAttempt;
        int waitForAmountOfFrames;
        bool goalNeedsUpdate;
        bool isBackAtOwnSquare;
        float offSetX;
        float offSetY;
        nearestBuildingTile dropOffTile;
        std::list<Cells> route;

};

extern std::vector<actors> listOfActors;

#endif // ACTORS_H
