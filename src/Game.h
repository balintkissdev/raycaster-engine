#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

#include "SDL_Deleter.h"
#include "Camera.h"
#include "RayCaster.h"

typedef std::vector< std::vector<int> > Map;

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

static const float BASE_MOVEMENT_SPEED = 0.035;
static const float RUN_MOVEMENT_SPEED = BASE_MOVEMENT_SPEED + 0.04;
static const float CURSOR_TURN_SPEED = 0.03;

// FIXME: Game class has strict coupling to SDL
class Game
{
    public:
        Game()
            : running_(false)
            , map_{}
            , overview_map_on(false)
            , movement_speed_(BASE_MOVEMENT_SPEED)
            , raycaster_(map_, WINDOW_WIDTH, WINDOW_HEIGHT)
            , camera_(4.5, 4.5, 1, 0, 0, -0.60, map_)
            , window_(nullptr)
            , renderer_(nullptr)
            , top_texture_(nullptr)
            {}
        ~Game(); 

        void init();
        void run();

    private:
        bool running_;
        Map map_;
        bool overview_map_on;
        float movement_speed_;

        RayCaster raycaster_;
        Camera camera_;

        SDL_Event e_;
        std::unique_ptr<SDL_Window, SDL_Deleter> window_;
        std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;

        std::unique_ptr<SDL_Texture, SDL_Deleter> top_texture_;
        std::vector< std::shared_ptr<SDL_Surface> > wall_textures_;

        SDL_Surface* loadSurface(std::string path);
        SDL_Texture* loadTexture(std::string path);

        Map loadMap(const std::string& path);

        void event();
        void update();
        void render();

        void drawMap();
};

#endif
