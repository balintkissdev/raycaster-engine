#include "SDLRenderer.h"

#include <cassert>
#include <iostream>

SDLRenderer::SDLRenderer() = default;

SDLRenderer::~SDLRenderer() = default;

bool SDLRenderer::initialize(
        const uint16_t screen_width,
        const uint16_t screen_height,
        const std::string& window_title)
{
    screen_width_ = screen_width;
    screen_height_ = screen_height;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        errorMessage_ = "Error initializing video: " + std::string(SDL_GetError());
        return false;
    }

    window_.reset(SDL_CreateWindow(
                window_title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                screen_width,
                screen_height,
                SDL_WINDOW_SHOWN
                ));
    if (!window_)
    {
        errorMessage_ = "Error creating window: " + std::string(SDL_GetError());
        return false;
    }

    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, 0));
    if (!renderer_)
    {
        errorMessage_ = "Error creating renderer: " + std::string(SDL_GetError());
        return false;
    }
    SDL_ShowCursor(SDL_DISABLE);

    streamableTexture_.reset(SDL_CreateTexture(renderer_.get(), SDL_GetWindowPixelFormat(window_.get()), SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height));
    if (!streamableTexture_)
    {
        errorMessage_ = SDL_GetError();
        return false;
    }

    return true;
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

void SDLRenderer::setDrawColor(
        const uint8_t red,
        const uint8_t green,
        const uint8_t blue,
        const uint8_t alpha)
{
    SDL_SetRenderDrawColor(renderer_.get(), red, green, blue, alpha);
}

void SDLRenderer::drawLine(
        const int x_start,
        const int y_start,
        const int x_end,
        const int y_end)
{
    SDL_RenderDrawLine(renderer_.get(), x_start, y_start, x_end, y_end);
}

void SDLRenderer::drawRectangle(
        const int x_position,
        const int y_position,
        const int width,
        const int height)
{
    const SDL_Rect rect{x_position, y_position, width, height};
    SDL_RenderDrawRect(renderer_.get(), &rect);
}

void SDLRenderer::fillRectangle(
        const int x_position,
        const int y_position,
        const int width,
        const int height)
{
    const SDL_Rect rect{x_position, y_position, width, height};
    SDL_RenderFillRect(renderer_.get(), &rect);
}

std::string SDLRenderer::errorMessage() const
{
    return errorMessage_;
}

void SDLRenderer::drawBuffer(uint32_t* drawBuffer)
{
    void *pixels = nullptr;
    int pitch;
    SDL_LockTexture(streamableTexture_.get(), nullptr, &pixels, &pitch);
    memcpy(pixels, drawBuffer, pitch * 768);
    SDL_UnlockTexture(streamableTexture_.get());

    SDL_RenderCopy(renderer_.get(), streamableTexture_.get(), nullptr, nullptr);
}

bool SDLRenderer::createTexture(Texture& textureOut, const int width, const int height, const char* file)
{
    textureOut.pixels.resize(width * height);

    std::string absolute_path = std::string(SDL_GetBasePath()) + std::string(file);
    SDLSurfacePtr loaded_surface(SDL_LoadBMP(absolute_path.c_str()));
    if (!loaded_surface )
    {
        std::cout << SDL_GetError() << std::endl;
        return false;
    }

    SDLSurfacePtr converted_surface(SDL_ConvertSurfaceFormat(loaded_surface.get(), SDL_GetWindowPixelFormat(window_.get()), 0));
    if (!converted_surface)
    {
        errorMessage_ = std::string("Error loading texture: " + std::string(SDL_GetError()));
        return false;
    }

    memcpy(textureOut.pixels.data(), converted_surface->pixels, converted_surface->pitch * converted_surface->h);
    textureOut.width = width;
    textureOut.height = height;
    textureOut.pitch = converted_surface->pitch;
    return true;
}
