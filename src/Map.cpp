#include "Map.h"

#include <fstream>
#include <iostream>
#include <sstream>

std::optional<Map> Map::create(const std::string& mapFilePath)
{
    std::ifstream file(mapFilePath, std::ios::in);
    if (file)
    {
        Map map;
        for (std::string line; std::getline(file, line);)
        {
            std::stringstream currentLine(line);
            for (size_t squareIndex; currentLine >> squareIndex;)
            {
                if (currentLine.peek() == ' ' || currentLine.peek() == ',')
                {
                    currentLine.ignore();
                }
                map.squareIndices_.push_back(squareIndex);

                if (map.rowCount_ == 0)
                {
                    ++map.columnCount_;
                }
            }
            ++map.rowCount_;
        }

        return map;
    }

    std::cerr << "Unable to load map file: " + mapFilePath;
    return std::nullopt;
}

Map::Map() = default;

size_t Map::position(const size_t x, const size_t y) const
{
    return squareIndices_[y + x * rowCount_];
}

size_t Map::rowCount() const
{
    return rowCount_;
}

size_t Map::columnCount() const
{
    return columnCount_;
}