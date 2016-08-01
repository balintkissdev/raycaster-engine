#ifndef WALL_TYPES_H
#define WALL_TYPES_H

#include <SDL2/SDL.h>

enum SquareType
{
    EMPTY_SPACE,
    RED_WALL,
    GREEN_WALL,
    BLUE_WALL,
    YELLOW_WALL
};

enum WallSide
{
    VERTICAL,
    HORIZONTAL
};

struct WallColor
{
    Uint8 r, g, b;
};

#endif
