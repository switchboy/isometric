#ifndef OBJECTS_H
#define OBJECTS_H
#include <vector>

class objects
{

public:
    objects(int type, int startXlocation, int startYLocation, int buildingId);
    void    createObject(int type);
    void    update();
    void    drawObjectFootprint(int type, int mouseWorldX, int mouseWorldY);
    void    drawObject(int i, int j);
    void    drawObjectSprite(int spriteNumber, int i, int j);
    int     getTypeOfResource();
    void    substractResource();

private:
    int     objectId;
    int     objectType;
    int     locationX;
    int     locationY;
    int     typeOfResource;
    int     resourceLeft;

};
extern std::vector<objects> listOfObjects;

#endif // OBJECTS_H
