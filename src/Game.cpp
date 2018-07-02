#include "Game.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>

#include "SDLRenderer.h"
#include "WallTypes.h"

#ifdef _WIN32
    const char FILE_SEPARATOR = '\\';
#else
    const char FILE_SEPARATOR = '/';
#endif


Game::Game()
    : running_(false)
    , map_{}
    , overview_map_on(false)
    , movement_speed_(BASE_MOVEMENT_SPEED)
    , renderer_(new SDLRenderer)
    , raycaster_(map_, WINDOW_WIDTH, WINDOW_HEIGHT)
    , camera_(4.5, 4.5, 1, 0, 0, -0.60, map_)
{}

Game::~Game()
{
    top_texture_.reset();
    renderer_.reset();
    SDL_Quit();
}

void Game::init()
{
    // Initialize SDL window and renderer
    bool success = renderer_->initialize(
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            "Press M for map, cursors to move and turn and "
            "WASD to move and strafe."
            );
    if (!success)
    {
        throw std::runtime_error(renderer_->errorMessage());
    }

    // Load map
    try
    {
        std::string map_path(
                std::string("resources") + FILE_SEPARATOR + "map" +
                FILE_SEPARATOR + "map.dat"
                );
        map_ = loadMap(map_path);
    }
    catch (std::runtime_error& e)
    {
        renderer_.reset();
        SDL_Quit();
        throw e;
    }

    // Load sky texture
    top_texture_.reset(renderer_->loadTexture(
                std::string("resources") + FILE_SEPARATOR + "textures" +
                FILE_SEPARATOR + "/dusk_sky_texture.bmp"
                ));
    if (!top_texture_)
    {
        renderer_.reset();
        SDL_Quit();
        throw std::runtime_error("Error loading image: " + renderer_->errorMessage());
    }

    // Set camera options
    camera_.rotationSpeed(CURSOR_TURN_SPEED);
}

Map Game::loadMap(const std::string& path)
{
    std::string absolute_path = std::string(SDL_GetBasePath()) + path.c_str();
    std::ifstream file(absolute_path, std::ios::in);
    if (file)
    {
        std::string line;
        std::vector< std::vector<int> > map;
        while (std::getline(file, line))
        {
            std::vector<int> row;
            std::stringstream current_line(line);
            int i;
            while (current_line >> i)
            {
                if (current_line.peek() == ' ' || current_line.peek() == ',')
                {
                    current_line.ignore();
                }
                row.push_back(i);
            }

            map.push_back(row);
        }

        return map;
    }
    throw std::runtime_error("Unable to load map file: " + path);
}

void Game::run()
{
    using namespace std::chrono;
    using namespace std::chrono_literals;

    constexpr nanoseconds TIME_STEP(16ms);

    high_resolution_clock::time_point previous_time = high_resolution_clock::now();
    nanoseconds lag(0ns);

    running_ = true;
    while (running_)
    {
        high_resolution_clock::time_point now = high_resolution_clock::now();
        auto delta = now - previous_time;
        previous_time = now;

        lag += delta;

        event();
        while (lag >= TIME_STEP)
        {
            update();
            lag -= TIME_STEP;
        }

        render();
    }
}

void Game::event()
{
    // TODO: Decouple SDL event handling
    const Uint8 * keystate = SDL_GetKeyboardState(nullptr);

    // FIXME: this code segment looks rather monolithic
    if (keystate[SDL_SCANCODE_LSHIFT])
    {
        movement_speed_ = RUN_MOVEMENT_SPEED;
    }
    else
    {
        movement_speed_ = BASE_MOVEMENT_SPEED;
    }

    if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
    {
        camera_.moveForward();
    }
    if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
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

void Game::update()
{
    camera_.movementSpeed(movement_speed_);
}

void Game::render()
{
    renderer_->clearScreen();

    raycaster_.drawTop(renderer_.get(), top_texture_.get());
    raycaster_.drawBottom(renderer_.get());
    raycaster_.drawWalls(renderer_.get(), camera_);

    if (overview_map_on)
    {
      drawMap();
    }

    renderer_->refreshScreen();
}

// TODO: this is a very basic map
void Game::drawMap()
{
    struct Rectangle
    {
      int x_position;
      int y_position;
      int width;
      int height;
    };

    Rectangle rect;

    // Draw blocks
    const int square_size = 32;
    for (int row = 0; row < static_cast<int>(map_.size()); ++row)
    {
        for (int column = 0; column < static_cast<int>(map_[row].size()); ++column)
        {
            WallColor wall_color;
            switch(map_[row][column])
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
            renderer_->setDrawColor(wall_color.red, wall_color.green, wall_color.blue);

            // Watch out: row/column is not the same as x/y. This was a source of a nasty bug.
            rect = {0 + square_size * column, 0 + square_size * row, square_size, square_size};
            renderer_->fillRectangle(rect.x_position, rect.y_position, rect.width, rect.height);
            renderer_->setDrawColor(0, 0, 0);
            renderer_->drawRectangle(rect.x_position, rect.y_position, rect.width, rect.height);
        }
    }

    // Draw player
    renderer_->setDrawColor(255, 255, 255);
    // HACK: need to change internal representation of the map instead switching x/y here
    rect = {
        square_size * static_cast<int>(camera_.position().y) + square_size / 4, 
        square_size * static_cast<int>(camera_.position().x) + square_size / 4, 
        square_size / 2, square_size / 2
    };
    renderer_->fillRectangle(rect.x_position, rect.y_position, rect.width, rect.height);
    renderer_->setDrawColor(0, 0, 0, 255);
    renderer_->drawRectangle(rect.x_position, rect.y_position, rect.width, rect.height);
}
