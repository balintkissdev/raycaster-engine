#ifndef RAYCASTER_H 
#define RAYCASTER_H

#include <vector>

#include <SDL2/SDL.h>

#include "Camera.h"

class RayCaster
{
    public:
        RayCaster(const std::vector< std::vector<int> >& map, const int width, const int height)
            : map_(map)
            , width_(width), height_(height)
            {}

        void drawCeilingAndFloor(SDL_Renderer* renderer);
        void drawWalls(SDL_Renderer* renderer, const Camera& camera);

    private:
        std::vector< std::vector<int> > map_;
        int width_, height_;
};

#endif
