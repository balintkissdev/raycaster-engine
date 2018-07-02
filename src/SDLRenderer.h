#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include "IRenderer.h"
#include "SDLDeleter.h"

#include <SDL2/SDL.h>

#include <memory>

struct SDL_Texture;

class SDLRenderer : public IRenderer {
    public:
        SDLRenderer();

        // IRenderer interface
        virtual ~SDLRenderer() override;
        virtual bool initialize(
                const uint16_t screen_width,
                const uint16_t screen_height,
                const std::string& window_title) override;
        virtual void clearScreen() override;
        virtual void refreshScreen() override;
        virtual void setDrawColor(
                const uint8_t red,
                const uint8_t green,
                const uint8_t blue,
                const uint8_t alpha) override;
        virtual void drawLine(
                const int x_start,
                const int y_start,
                const int x_end,
                const int y_end) override;
        virtual void drawRectangle(
                const int x_position,
                const int y_position,
                const int width,
                const int height) override;
        virtual void fillRectangle(
                const int x_position,
                const int y_position,
                const int width,
                const int height) override;
        virtual SDL_Texture* loadTexture(const std::string& path) override;
        virtual void drawTexture(SDL_Texture* texture) override;
        virtual std::string errorMessage() const override;

    private:
        using SDLWindowPtr = std::unique_ptr<SDL_Window, SDLDeleter>;
        using SDLRendererPtr = std::unique_ptr<SDL_Renderer, SDLDeleter>;

        SDLWindowPtr window_;
        SDLRendererPtr renderer_;
        std::string errorMessage_;
};

#endif  // SDLRENDERER_H

