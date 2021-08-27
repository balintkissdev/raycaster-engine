#include "Game.h"

#include "SDLRenderer.h"

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef __EMSCRIPTEN__
extern "C" void emscriptenCallback(Game* game)
{
    game->runEmscriptenIteration();
}
#endif

Game::Game()
    : camera_(INITIAL_POSITION, INITIAL_DIRECTION, INITIAL_FIELD_OF_VIEW, map_)
    , raycaster_(camera_, map_)
    , renderer_(std::make_unique<SDLRenderer>())
    , movementSpeed_(BASE_MOVEMENT_SPEED)
{
}

Game::~Game()
{
    renderer_.reset();
    SDL_Quit();
}

bool Game::init()
{
    bool success = renderer_->init(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "Press M for map, cursors to move and turn and "
        "WASD to move and strafe.");
    if (!success)
    {
        return false;
    }
    atexit(SDL_Quit);

    std::optional<Map> map = Map::create("resources/map/map.dat");
    if (!map.has_value())
    {
        return false;
    }
    map_ = map.value();

    if (!raycaster_.init(*renderer_))
    {
        std::cerr << "Error initializing raycaster engine." << std::endl;
        return false;
    }

    camera_.rotationSpeed(CURSOR_TURN_SPEED);

    return true;
}

int Game::run()
{
    running_ = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(reinterpret_cast<em_arg_callback_func>(emscriptenCallback), this, 0, true);
#else
    using namespace std::chrono;
    using namespace std::chrono_literals;

    constexpr nanoseconds timeStep(16ms);

    high_resolution_clock::time_point previousTime = high_resolution_clock::now();
    nanoseconds lag(0ns);

    while (running_)
    {
        high_resolution_clock::time_point currentTime = high_resolution_clock::now();
        auto elapsedTime = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsedTime;

        event();
        while (lag >= timeStep)
        {
            update();
            lag -= timeStep;
        }

        render();
    }
#endif

    return EXIT_SUCCESS;
}

#ifdef __EMSCRIPTEN__
void Game::runEmscriptenIteration()
{
    event();
    update();
    render();
}
#endif

void Game::event()
{
    // TODO: Decouple SDL event handling
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    // FIXME: this code segment looks rather monolithic
    if (keystate[SDL_SCANCODE_LSHIFT])
    {
        movementSpeed_ = RUN_MOVEMENT_SPEED;
    }
    else
    {
        movementSpeed_ = BASE_MOVEMENT_SPEED;
    }

    if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
    {
        camera_.move(Camera::DIRECTION_FORWARD);
    }
    if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
    {
        camera_.move(Camera::DIRECTION_BACKWARD);
    }
    if (keystate[SDL_SCANCODE_LEFT])
    {
        camera_.turn(Camera::DIRECTION_LEFT);
    }
    if (keystate[SDL_SCANCODE_RIGHT])
    {
        camera_.turn(Camera::DIRECTION_RIGHT);
    }
    if (keystate[SDL_SCANCODE_A])
    {
        camera_.strafe(Camera::DIRECTION_LEFT);
    }
    if (keystate[SDL_SCANCODE_D])
    {
        camera_.strafe(Camera::DIRECTION_RIGHT);
    }

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                running_ = false;
                break;

            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running_ = false;
                        break;
                    case SDLK_m:
                        raycaster_.toggleMapDraw();
                        break;
                }
        }
    }
}

void Game::update()
{
    camera_.movementSpeed(movementSpeed_);
}

void Game::render()
{
    renderer_->clearScreen();
    raycaster_.drawEverything(*renderer_);
    renderer_->refreshScreen();
}

int main(int /*argc*/, char** /*argv*/)
{
    Game game;
    if (!game.init())
    {
        return EXIT_FAILURE;
    }

    return game.run();
}
