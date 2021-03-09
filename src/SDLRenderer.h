#ifndef SDLRENDERER_H
#define SDLRENDERER_H

#include "IRenderer.h"

#include <SDL2/SDL.h>

#include <memory>

class SDLRenderer : public IRenderer {
    public:
        SDLRenderer();

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
        virtual std::string errorMessage() const override;
        virtual void drawBuffer(uint32_t *drawBuffer) override;
        virtual bool createTexture(Texture& textureOut, const int width, const int height, const char* file) override;

    private:
        struct SDLDeleter
        {
           void operator()(SDL_Window* window) { SDL_DestroyWindow(window); }
           void operator()(SDL_Renderer* render) { SDL_DestroyRenderer(render); }
           void operator()(SDL_Surface* surface) { SDL_FreeSurface(surface); }
           void operator()(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
        };

        template<typename SDLType>
        using SDLPtr = std::unique_ptr<SDLType, SDLDeleter>;
        using SDLSurfacePtr = SDLPtr<SDL_Surface>;

        using SDLWindowPtr = SDLPtr<SDL_Window>;
        using SDLRendererPtr = SDLPtr<SDL_Renderer>;
        using SDLTexturePtr = SDLPtr<SDL_Texture>;

        SDLWindowPtr window_;
        SDLRendererPtr renderer_;
        std::string errorMessage_;
        uint16_t screen_width_;
        uint16_t screen_height_;
        SDLTexturePtr streamableTexture_;
};

#endif  // SDLRENDERER_H

