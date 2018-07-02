#ifndef WALL_TYPES_H
#define WALL_TYPES_H

#include <cstdint>

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
    uint8_t red, green, blue;
};

#endif
