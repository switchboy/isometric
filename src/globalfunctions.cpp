#include "globalfunctions.h"
#include "gamestate.h"



bool compareCord(const rectangleCord& lhs, rectangleCord& rhs)
{
    if(lhs.x == rhs.x && lhs.y == rhs.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool rectCord(const rectangleCord& lhs, rectangleCord& rhs)
{
    return(lhs.y<rhs.y);
}

bool sortCordByX(const rectangleCord& lhs, rectangleCord& rhs)
{
    return(lhs.x<rhs.x);
}

int worldSpace(int x, int y, bool getX)
{
    int wX = mapOffsetX*64 + ((x - y) * (64/2));
    int wY = mapOffsetY*32 + ((x + y) * (32/2));
    if(getX)
    {
        return wX;
    }
    else
    {
        return wY;
    }
}

int miniMapSpace(int x, int y, bool getX)
{
    int wX = mapOffsetX*20 + (x - y) * (20/2);
    int wY = mapOffsetY*10 + (x + y) * (10/2);
    if(getX)
    {
        return wX;
    }
    else
    {
        return wY;
    }
}


