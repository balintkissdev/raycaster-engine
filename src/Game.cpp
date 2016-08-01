#include "Game.h"

#include <iostream>
#include <stdexcept>

#include "WallTypes.h"

#ifdef _WIN32
    const char FILE_SEPARATOR = '\\';
#else
    const char FILE_SEPARATOR = '/';
#endif

void Game::init()
{
    // Initialize SDL window and renderer
    SDL_Init(SDL_INIT_VIDEO);
    window_.reset(SDL_CreateWindow(
                "Press M for map, cursors to move and turn and WASD to move and strafe.",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_SHOWN
            ));
    if (!window_)
    {
        SDL_Quit();
        std::string sdl_error_msg(SDL_GetError());
        throw std::runtime_error("Error creating window" + sdl_error_msg);
    }

    // TODO: Add selectable Hardware rendering capability
    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_SOFTWARE));
    if (!renderer_)
    {
        window_.reset();
        std::string sdl_error_msg(SDL_GetError());
        SDL_Quit();
        throw std::runtime_error("Error creating renderer: " + sdl_error_msg);
    }

    // Load sky texture
    top_texture_.reset(loadTexture(std::string("resources") + FILE_SEPARATOR + "textures" + FILE_SEPARATOR + "/dusk_sky_texture.bmp"));
    if (!top_texture_)
    {
        renderer_.reset();
        window_.reset();
        std::string sdl_error_msg(SDL_GetError());
        SDL_Quit();
        throw std::runtime_error("Error loading image: " + sdl_error_msg);
    }

    // Load wall textures
    //auto shared_surface_deleter = [](SDL_Surface* surface) { SDL_FreeSurface(surface); };   // HACK
    //wall_textures_.emplace_back(loadSurface("resources/textures/brick1.bmp"), shared_surface_deleter);
    //wall_textures_.emplace_back(loadSurface("resources/textures/brick2.bmp"), shared_surface_deleter);
    //wall_textures_.emplace_back(loadSurface("resources/textures/stone1.bmp"), shared_surface_deleter);
    //wall_textures_.emplace_back(loadSurface("resources/textures/stone2.bmp"), shared_surface_deleter);
    // TODO: error handling and cleanup code for multiple texture

    SDL_ShowCursor(SDL_DISABLE);
}

Game::~Game()
{
    for (auto& t : wall_textures_)
    {
        t.reset();
    }

    top_texture_.reset();
    renderer_.reset();
    window_.reset();
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

SDL_Texture* Game::loadTexture(std::string path)
{
    std::string absolute_path = std::string(SDL_GetBasePath()) + path.c_str();
    SDL_Surface* tmp_surface = SDL_LoadBMP(absolute_path.c_str());
    if (!tmp_surface)
    {
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_.get(), tmp_surface);
    SDL_FreeSurface(tmp_surface);
    return texture;
}

SDL_Surface* Game::loadSurface(std::string path)
{
    std::string absolute_path = std::string(SDL_GetBasePath()) + path.c_str();
    SDL_Surface* surface = SDL_LoadBMP(absolute_path.c_str());
    if (surface)
    {
        surface = SDL_ConvertSurface(surface, SDL_GetWindowSurface(window_.get())->format, 0);

        if (surface)
        {
            return surface;
        }
        SDL_FreeSurface(surface);
    }

    return nullptr;
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
    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer_.get());

    raycaster_.drawCeilingAndFloor(renderer_.get(), top_texture_.get());
    raycaster_.drawWalls(renderer_.get(), camera_, wall_textures_);

    if (overview_map_on) drawMap();

    // Swap buffer
    SDL_RenderPresent(renderer_.get());
}

void Game::drawMap()
{
    // Draw blocks
    const int square_size = 32;
    SDL_Rect rect;
    for (int x = 0; x < static_cast<int>(map_.size()); ++x)
    {
        for (int y = 0; y < static_cast<int>(map_[x].size()); ++y)
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
            SDL_SetRenderDrawColor(renderer_.get(), wall_color.r, wall_color.g, wall_color.b, 255);
            rect = {0 + square_size * x, 0 + square_size * y, square_size, square_size};
            SDL_RenderFillRect(renderer_.get(), &rect);
            SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer_.get(), &rect);
        }
    }

    // Draw player
    SDL_SetRenderDrawColor(renderer_.get(), 255, 255, 255, 255);
    rect = {
        square_size * static_cast<int>(camera_.xPos()) + square_size / 4, 
        square_size * static_cast<int>(camera_.yPos()) + square_size / 4, 
        square_size / 2, square_size / 2
    };
    SDL_RenderFillRect(renderer_.get(), &rect);
    SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer_.get(), &rect);
}
