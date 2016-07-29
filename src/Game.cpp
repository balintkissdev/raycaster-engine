#include "Game.h"

#include <iostream>
#include <stdexcept>

void Game::init()
{
    // Initialize SDL window and renderer
    SDL_Init(SDL_INIT_VIDEO);
    window_ = SDL_CreateWindow(
                "Raycaster",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_SHOWN
            );
    if (!window_)
    {
        SDL_Quit();
        throw std::runtime_error("Error creating window");
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_)
    {
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw std::runtime_error("Error creating renderer");
    }

    // Load sky texture
    SDL_Surface* tmp_surface = SDL_LoadBMP("dusk_sky_texture.bmp");
    if (!tmp_surface)
    {
        SDL_DestroyWindow(window_);
        SDL_DestroyRenderer(renderer_);
        SDL_Quit();
        throw std::runtime_error("Error loading image.");
    }
    top_texture_ = SDL_CreateTextureFromSurface(renderer_, tmp_surface);
    SDL_FreeSurface(tmp_surface);
}

Game::~Game()
{
    SDL_DestroyTexture(top_texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::run()
{
    running_ = true;
    
    double current_time = 0;
    double previous_time = 0;

    while (running_)
    {
        previous_time = current_time;
        current_time = SDL_GetTicks();
        double frame_time = (current_time - previous_time) / 1000.0;
        SDL_SetWindowTitle(window_, std::to_string(1.0 / frame_time).c_str());  // Print framerate

        event();
        update(frame_time);
        render();
    }
}

void Game::event()
{

    const Uint8 * keystate = SDL_GetKeyboardState(nullptr);

    if (keystate[SDL_SCANCODE_UP])
    {
        camera_.moveForward();
    }
    if (keystate[SDL_SCANCODE_DOWN])
    {
        camera_.moveBackward();
    }
    if (keystate[SDL_SCANCODE_LEFT])
    {
        camera_.moveLeft();
    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        camera_.moveRight();
    }
    while (SDL_PollEvent(&e_))
    {
        switch (e_.type)
        {
            case SDL_QUIT:
                running_ = false;
                break;

            case SDL_KEYDOWN:
                switch (e_.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running_ = false;
                        break;
                }
        }
    }
}

void Game::update(const double frame_time)
{
    camera_.movSpeed(frame_time * 5.0);
    camera_.rotSpeed(frame_time * 3.0);
}

void Game::render()
{
    // Clear
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    raycaster_.drawCeilingAndFloor(renderer_, top_texture_);
    raycaster_.drawWalls(renderer_, camera_);

    // Swap buffer
    SDL_RenderPresent(renderer_);
}
