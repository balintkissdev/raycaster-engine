#ifndef SCREEN_H
#define SCREEN_H

#include <vector>
#include <SDL2/SDL.h>
#include "Camera.h"

class Screen
{
    public:
        Screen(const std::vector< std::vector<int> >& map, const int width, const int height)
            : map_(map)
            , width_(width), height_(height)
            {}

        void update(const Camera& camera, SDL_Renderer* renderer);
        void render(SDL_Renderer* renderer);

    private:
        std::vector< std::vector<int> > map_;
        int width_, height_;
};

#endif
