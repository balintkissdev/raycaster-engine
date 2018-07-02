#include "SDLRenderer.h"

SDLRenderer::SDLRenderer() = default;

SDLRenderer::~SDLRenderer() = default;

bool SDLRenderer::initialize(
        const uint16_t screen_width,
        const uint16_t screen_height,
        const std::string& window_title)
{
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

SDL_Texture* SDLRenderer::loadTexture(const std::string& path)
{
    std::string absolute_path = std::string(SDL_GetBasePath()) + path.c_str();
    SDL_Surface* tmp_surface = SDL_LoadBMP(absolute_path.c_str());
    if (!tmp_surface)
    {
        errorMessage_ = std::string("Error loading texture: " + std::string(SDL_GetError()));
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_.get(), tmp_surface);
    SDL_FreeSurface(tmp_surface);
    return texture;
}

void SDLRenderer::drawTexture(SDL_Texture* texture)
{
    SDL_RenderCopy(renderer_.get(), texture, nullptr, nullptr);
}

std::string SDLRenderer::errorMessage() const
{
    return errorMessage_;
}

