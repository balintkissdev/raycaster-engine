#ifndef WALL_TYPES_H
#define WALL_TYPES_H

#include <cstdint>

// TODO: Not relevant since texturing, so these will be removed.
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

// TODO: Not relevant since texturing, so these will be removed.
struct WallColor
{
    uint8_t red, green, blue;
};

#endif
