#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h> // TODO: Abstract away

#include "Camera.h"
#include "IRenderer.h"
#include "RayCaster.h"

typedef std::vector< std::vector<int> > Map;

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

static const float BASE_MOVEMENT_SPEED = 0.035;
static const float RUN_MOVEMENT_SPEED = BASE_MOVEMENT_SPEED + 0.04;
static const float CURSOR_TURN_SPEED = 0.03;

class Game
{
    public:
        Game();
        ~Game();

        void init();
        void run();

    private:
        bool running_;
        Map map_;
        bool overview_map_on;
        float movement_speed_;

        std::unique_ptr<IRenderer> renderer_;
        RayCaster raycaster_;
        Camera camera_;

        SDL_Event e_;

        Map loadMap(const std::string& path);

        void event();
        void update();
        void render();

        void drawMap();
};

#endif
