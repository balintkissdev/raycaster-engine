#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <vector>

struct Texture
{
    size_t width;
    size_t height;
    size_t pitch;
    std::vector<uint32_t> texels;
};

#endif