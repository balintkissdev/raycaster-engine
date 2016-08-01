#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>

#include "SDL_Deleter.h"
#include "Camera.h"
#include "RayCaster.h"

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

// FIXME: Game class has strict coupling to SDL
class Game
{
    public:
        Game()
            : running_(false)
            , map_{ {1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2},
                    {1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
                    {1, 0, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 2},
                    {1, 0, 3, 0, 0, 0, 3, 0, 2, 0, 0, 0, 0, 0, 2},
                    {1, 0, 3, 0, 0, 0, 3, 0, 2, 2, 2, 0, 2, 2, 2},
                    {1, 0, 3, 0, 0, 0, 3, 0, 2, 0, 0, 0, 0, 0, 2},
                    {1, 0, 3, 3, 0, 3, 3, 0, 2, 0, 0, 0, 0, 0, 2},
                    {1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2},
                    {1, 1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 0, 4, 4, 4},
                    {1, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 4},
                    {1, 0, 0, 0, 0, 0, 1, 4, 0, 0, 0, 0, 0, 0, 4},
                    {1, 0, 0, 2, 0, 0, 1, 4, 0, 3, 3, 3, 3, 0, 4},
                    {1, 0, 0, 0, 0, 0, 1, 4, 0, 3, 3, 3, 3, 0, 4},
                    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
                    {1, 1, 1, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 4, 4} }
            , overview_map_on(false)
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
        std::vector< std::vector<int> > map_;
        bool overview_map_on;

        RayCaster raycaster_;
        Camera camera_;

        SDL_Event e_;
        std::unique_ptr<SDL_Window, SDL_Deleter> window_;
        std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;

        std::unique_ptr<SDL_Texture, SDL_Deleter> top_texture_;
        std::vector< std::shared_ptr<SDL_Surface> > wall_textures_;

        SDL_Surface* loadSurface(std::string path);
        SDL_Texture* loadTexture(std::string path);

        void event();
        void update(const double frame_time);
        void render();

        void drawMap();
};

#endif
