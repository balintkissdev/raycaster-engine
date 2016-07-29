#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL.h>

#include "Camera.h"
#include "RayCaster.h"

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

class Game
{
    public:
        Game()
            : running_(false)
            , window_(0)
            , renderer_(0)
            , pixels_{}
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
            , raycaster_(map_, WINDOW_WIDTH, WINDOW_HEIGHT)
            , camera_(4.5, 4.5, 1, 0, 0, -0.60, map_)
            {}
        ~Game(); 

        void init();
        void run();

    private:
        bool running_;
        SDL_Window* window_;
        SDL_Renderer* renderer_;

        SDL_Event e_;

        std::vector<int> pixels_;
        std::vector< std::vector<int> > map_;

        RayCaster raycaster_;
        Camera camera_;

        void render();
        void event();
};

#endif
