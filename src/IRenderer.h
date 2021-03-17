#ifndef IRENDERER_H
#define IRENDERER_H

#include "Texture.h"

#include <string>

// TODO: Comment interface
class IRenderer {
    public:
        virtual ~IRenderer() = default;
        virtual bool initialize(
                const uint16_t screen_width,
                const uint16_t screen_height,
                const std::string& window_title) = 0;
        virtual void clearScreen() = 0;
        virtual void refreshScreen() = 0;
        virtual void setDrawColor(
                const uint8_t red,
                const uint8_t green,
                const uint8_t blue,
                const uint8_t alpha = 255) = 0;
        virtual void drawLine(
                const int x_start,
                const int y_start,
                const int x_end,
                const int y_end) = 0;
        virtual void drawRectangle(
                const int x_position,
                const int y_position,
                const int width,
                const int height) = 0;
        virtual void fillRectangle(
                const int x_position,
                const int y_position,
                const int width,
                const int height) = 0;
        virtual std::string errorMessage() const = 0;
        virtual void drawBuffer(uint32_t *drawBuffer) = 0;
        virtual bool createTexture(Texture& textureOut, const int width, const int height, const char* file) = 0;
};

#endif  // IRENDERER_H