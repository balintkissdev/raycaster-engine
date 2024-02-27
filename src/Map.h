#pragma once

#include "Vector2.h"

#include <optional>
#include <string>
#include <vector>

class Map
{
public:
    static constexpr size_t EMPTY_SQUARE_INDEX = 0;

    static std::optional<Map> create(const std::string& mapFilePath);

    Map();

    [[nodiscard]] size_t position(const size_t x, const size_t y) const;
    [[nodiscard]] size_t rowCount() const;
    [[nodiscard]] size_t columnCount() const;

private:
    std::vector<size_t> squareIndices_;
    size_t rowCount_{0}, columnCount_{0};
};

