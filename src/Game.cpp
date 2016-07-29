#include "Game.h"

#include <iostream>
#include <stdexcept>

// FIXME: duplicated code
enum SquareType
{
    EMPTY_SPACE,
    RED_WALL,
    GREEN_WALL,
    BLUE_WALL,
    YELLOW_WALL
};

struct WallColor
{
    int r, g, b;
};

void Game::init()
{
    // Initialize SDL window and renderer
    SDL_Init(SDL_INIT_VIDEO);
    window_ = SDL_CreateWindow(
                "Press M for map",
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

    SDL_ShowCursor(SDL_DISABLE);
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
        //SDL_SetWindowTitle(window_, std::to_string(1.0 / frame_time).c_str());  // Print framerate

        event();
        update(frame_time);
        render();
    }
}

void Game::event()
{
    const Uint8 * keystate = SDL_GetKeyboardState(nullptr);

    // FIXME: this code segment looks rather monolithic
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
        camera_.turnLeft();
    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        camera_.turnRight();
    }
    if (keystate[SDL_SCANCODE_W])
    {
        camera_.moveForward();
    }
    if (keystate[SDL_SCANCODE_S])
    {
        camera_.moveBackward();
    }
    if (keystate[SDL_SCANCODE_A])
    {
        camera_.strafeLeft();
    }
    if (keystate[SDL_SCANCODE_D])
    {
        camera_.strafeRight();
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
                    case SDLK_m:
                        overview_map_on = !overview_map_on;
                        break;
                }
        }
    }
}

void Game::update(const double frame_time)
{
    camera_.movSpeed(frame_time * 5.0);
    camera_.rotSpeed(frame_time * 3.0);

    //SDL_SetWindowTitle(window_, 
    //        (std::to_string(camera_.xPos()) + ":" + std::to_string(camera_.yPos())).c_str());
}

void Game::render()
{
    // Clear
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    raycaster_.drawCeilingAndFloor(renderer_, top_texture_);
    raycaster_.drawWalls(renderer_, camera_);

    if (overview_map_on) drawMap();

    // Swap buffer
    SDL_RenderPresent(renderer_);
}

void Game::drawMap()
{
    // Draw blocks
    const int square_size = 32;
    SDL_Rect rect;
    for (int x = 0; x < map_.size(); ++x)
    {
        for (int y = 0; y < map_[x].size(); ++y)
        {
            WallColor wall_color;
            switch(map_[x][y])
            {
                case EMPTY_SPACE:
                    wall_color = { 160, 160, 160 };
                    break;
                case RED_WALL:
                    wall_color = { 255, 0, 0 };
                    break;
                case GREEN_WALL:
                    wall_color = { 0, 255, 0 };
                    break;
                case BLUE_WALL:
                    wall_color = { 0, 0, 255 };
                    break;
                case YELLOW_WALL:
                    wall_color = { 255, 255, 0 };
                    break;
            }
            SDL_SetRenderDrawColor(renderer_, wall_color.r, wall_color.g, wall_color.b, 255);
            rect = {0 + square_size * x, 0 + square_size * y, square_size, square_size};
            SDL_RenderFillRect(renderer_, &rect);
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer_, &rect);
        }
    }

    // Draw player
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    rect = {
        square_size * static_cast<int>(camera_.xPos()) + square_size / 4, 
        square_size * static_cast<int>(camera_.yPos()) + square_size / 4, 
        square_size / 2, square_size / 2
    };
    SDL_RenderFillRect(renderer_, &rect);
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer_, &rect);
}
