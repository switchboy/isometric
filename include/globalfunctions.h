#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H
#define MAP_WIDTH	200
#define MAP_HEIGHT	200

extern int mapOffsetX;
extern int mapOffsetY;

struct mouseWorldCord
{
    int x;
    int y;
};

static mouseWorldCord toWorldMousePosition(int mouseX, int mouseY);

struct rectangleCord
{
    int x, y;
};

bool rectCord (const rectangleCord& lhs, rectangleCord& rhs);
bool compareCord(const rectangleCord& lhs, rectangleCord& rhs);
bool sortCordByX(const rectangleCord& lhs, rectangleCord& rhs);
int worldSpace(int x, int y, bool getX);
bool compareCord(const rectangleCord& lhs, rectangleCord& rhs);
bool rectCord(const rectangleCord& lhs, rectangleCord& rhs);
bool sortCordByX(const rectangleCord& lhs, rectangleCord& rhs);
int worldSpace(int x, int y, bool getX);
int miniMapSpace(int x, int y, bool getX);



#endif // GLOBALFUNCTIONS_H
