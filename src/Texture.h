#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

struct Texture
{
    size_t width;
    size_t height;
    size_t pitch;
    std::vector<uint32_t> texels;
};

