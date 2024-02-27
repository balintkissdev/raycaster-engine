#include "SDLRenderer.h"

#include "Texture.h"

#include <iostream>

SDLRenderer::~SDLRenderer() = default;

bool SDLRenderer::init(const uint16_t screenWidth, const uint16_t screenHeight, const std::string& windowTitle)
{
    screenWidth_ = screenWidth;
    screenHeight_ = screenHeight;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Error initializing video: " + std::string(SDL_GetError());
        return false;
    }

    window_.reset(SDL_CreateWindow(
        windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN));
    if (!window_)
    {
        std::cerr << "Error creating window: " + std::string(SDL_GetError());
        return false;
    }

    renderer_.reset(SDL_CreateRenderer(
        window_.get(),
        -1,
#ifdef __EMSCRIPTEN__
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC  // TODO: Not sure if makes difference in speed.
#else
        0
#endif
        ));
    if (!renderer_)
    {
        std::cerr << "Error creating renderer: " + std::string(SDL_GetError());
        return false;
    }
    SDL_ShowCursor(SDL_DISABLE);

    screenTexture_.reset(SDL_CreateTexture(
        renderer_.get(),
        SDL_GetWindowPixelFormat(window_.get()),
        SDL_TEXTUREACCESS_STREAMING,
        screenWidth,
        screenHeight));
    if (!screenTexture_)
    {
        std::cerr << SDL_GetError();
        return false;
    }

    return true;
}

uint16_t SDLRenderer::screenWidth() const
{
    return screenWidth_;
}

uint16_t SDLRenderer::screenHeight() const
{
    return screenHeight_;
}

void SDLRenderer::clearScreen()
{
    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer_.get());
}

void SDLRenderer::refreshScreen()
{
    SDL_RenderPresent(renderer_.get());
}

void SDLRenderer::setDrawColor(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
{
    SDL_SetRenderDrawColor(renderer_.get(), red, green, blue, alpha);
}

void SDLRenderer::drawLine(const int xStart, const int yStart, const int xEnd, const int yEnd)
{
    SDL_RenderDrawLine(renderer_.get(), xStart, yStart, xEnd, yEnd);
}

void SDLRenderer::drawRectangle(const int x, const int y, const int width, const int height)
{
    const SDL_Rect rect{x, y, width, height};
    SDL_RenderDrawRect(renderer_.get(), &rect);
}

void SDLRenderer::fillRectangle(const int x, const int y, const int width, const int height)
{
    const SDL_Rect rect{x, y, width, height};
    SDL_RenderFillRect(renderer_.get(), &rect);
}

void SDLRenderer::drawBuffer(uint32_t* drawBuffer)
{
    void* screenTexturePixels = nullptr;
    int screenTexturePitch;
    SDL_LockTexture(screenTexture_.get(), nullptr, &screenTexturePixels, &screenTexturePitch);
    memcpy(screenTexturePixels, drawBuffer, screenTexturePitch * screenHeight_);
    SDL_UnlockTexture(screenTexture_.get());

    SDL_RenderCopy(renderer_.get(), screenTexture_.get(), nullptr, nullptr);
}

std::optional<Texture> SDLRenderer::createTexture(const std::string& textureFilePath)
{
    SDLSurfacePtr loadedSurface(SDL_LoadBMP(textureFilePath.c_str()));
    if (!loadedSurface)
    {
        std::cout << SDL_GetError() << std::endl;
        return std::nullopt;
    }

    SDLSurfacePtr convertedSurface(
        SDL_ConvertSurfaceFormat(loadedSurface.get(), SDL_GetWindowPixelFormat(window_.get()), 0));
    if (!convertedSurface)
    {
        std::cerr << std::string("Error loading texture: " + std::string(SDL_GetError()));
        return std::nullopt;
    }

    Texture texture;
    texture.width = convertedSurface->w;
    texture.height = convertedSurface->h;
    texture.pitch = convertedSurface->pitch;
    texture.texels.resize(texture.width * texture.height);
    memcpy(texture.texels.data(), convertedSurface->pixels, texture.pitch * texture.height);
    return texture;
}
