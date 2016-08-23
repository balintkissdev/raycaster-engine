#ifndef RAYCASTER_H 
#define RAYCASTER_H

#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include "SDL_Deleter.h"
#include "WallTypes.h"
#include "MathLib.h"
#include "Camera.h"

class RayCaster
{
    public:
        RayCaster(const std::vector< std::vector<int> >& map, const int width, const int height)
            : map_(map)
            , width_(width), height_(height)
            {}

        void drawCeilingAndFloor(SDL_Renderer* renderer);
        void drawCeilingAndFloor(SDL_Renderer* renderer, SDL_Texture* top_texture); 
        void drawWalls(SDL_Renderer* renderer, 
                const Camera& camera,
                const std::vector< std::shared_ptr<SDL_Surface> >& wall_textures
        );

    private:
        const std::vector< std::vector<int> >& map_;
        int width_, height_;

        void drawPlainColoredStripe(SDL_Renderer* renderer, 
                const int x, 
                const mymath::Point2d<int>& square_on_map, 
                const int draw_start, const int draw_end, 
                const int side
        );

        WallColor extractPixelColor(SDL_Surface* wall, const mymath::Point2d<int>& pixel_position);
};

#endif
