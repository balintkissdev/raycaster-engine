#pragma once

#include "Texture.h"

#include <optional>
#include <string>

class IRenderer
{
public:
    virtual ~IRenderer() = default;
    virtual bool init(const uint16_t screenWidth, const uint16_t screenHeight, const std::string& windowTitle) = 0;

    [[nodiscard]] virtual uint16_t screenWidth() const = 0;
    [[nodiscard]] virtual uint16_t screenHeight() const = 0;

    virtual void clearScreen() = 0;
    virtual void refreshScreen() = 0;

    virtual void setDrawColor(
        const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha = 255) = 0;
    virtual void drawLine(const int xStart, const int yStart, const int xEnd, const int yEnd) = 0;
    virtual void drawRectangle(const int x, const int y, const int width, const int height) = 0;
    virtual void fillRectangle(const int x, const int y, const int width, const int height) = 0;

    virtual void drawBuffer(uint32_t* drawBuffer) = 0;

    // TODO: Remove SDL dependency with std_image and move to Texture.cpp as factory method.
    virtual std::optional<Texture> createTexture(const std::string& textureFilePath) = 0;
};

