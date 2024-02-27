#pragma once

#include "IRenderer.h"

#include <SDL2/SDL.h>

#include <memory>

class SDLRenderer : public IRenderer
{
public:
    ~SDLRenderer() override;
    bool init(const uint16_t screenWidth, const uint16_t screenHeight, const std::string& windowTitle) override;

    [[nodiscard]] uint16_t screenWidth() const override;
    [[nodiscard]] uint16_t screenHeight() const override;

    void clearScreen() override;
    void refreshScreen() override;

    void setDrawColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha) override;
    void drawLine(const int xStart, const int yStart, const int xEnd, const int yEnd) override;
    void drawRectangle(const int x, const int y, const int width, const int height) override;
    void fillRectangle(const int x, const int y, const int width, const int height) override;

    void drawBuffer(uint32_t* drawBuffer) override;

    std::optional<Texture> createTexture(const std::string& textureFilePath) override;

private:
    struct SDLDeleter
    {
        void operator()(SDL_Window* window) { SDL_DestroyWindow(window); }
        void operator()(SDL_Renderer* render) { SDL_DestroyRenderer(render); }
        void operator()(SDL_Surface* surface) { SDL_FreeSurface(surface); }
        void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
    };

    template <typename SDLType>
    using SDLPtr = std::unique_ptr<SDLType, SDLDeleter>;

    using SDLSurfacePtr = SDLPtr<SDL_Surface>;
    using SDLWindowPtr = SDLPtr<SDL_Window>;
    using SDLRendererPtr = SDLPtr<SDL_Renderer>;
    using SDLTexturePtr = SDLPtr<SDL_Texture>;

    SDLWindowPtr window_;
    SDLRendererPtr renderer_;
    uint16_t screenWidth_;
    uint16_t screenHeight_;
    SDLTexturePtr screenTexture_;
};

