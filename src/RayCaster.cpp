#include "RayCaster.h"

#include <cmath>
#include "Camera.h"
#include "MathLib.h"


enum SquareColor
{
    EMPTY_SPACE,
    RED_WALL,
    GREEN_WALL,
    BLUE_WALL,
    YELLOW_WALL
};

enum WallSide
{
    VERTICAL,
    HORIZONTAL
};

struct WallColor
{
    int r, g, b;
};

void RayCaster::drawCeilingAndFloor(SDL_Renderer* renderer)
{
    // Ceiling
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect rect = {0, 0, width_, height_/ 2};
    SDL_RenderFillRect(renderer, &rect);

    // Floor
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    rect = {0, height_ / 2, width_, height_};
    SDL_RenderFillRect(renderer, &rect);
};

/**
 * Calculate how the screen should look to the user based on their position on the map
 */
void RayCaster::drawWalls(SDL_Renderer* renderer, const Camera& camera) {

    // Cast rays
    for(int x = 0; x < width_; ++x)
    {
        double cameraX = 2 * x / (double)(width_) - 1;       // x coordinate of current vertical stripe of camera plane

        // Vectors for ray
        mymath::Vector2d<double> ray_vector(camera.xDir() + camera.xPlane() * cameraX, 
                                            camera.yDir() + camera.yPlane() * cameraX);

        // Map position
        int mapX = (int)camera.xPos();
        int mapY = (int)camera.yPos();

        // Length of ry from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        // Length of ray from one side to next in map
        double deltaDistX = std::sqrt( 1 + (ray_vector.y * ray_vector.y) / (ray_vector.x * ray_vector.x) );
        double deltaDistY = std::sqrt( 1 + (ray_vector.x * ray_vector.x) / (ray_vector.y * ray_vector.y) );

        double perpWallDist;    // Distance from player and the first wall
        int stepX, stepY;       // Direction to go in x and y
        bool hit = false;       // Was a wall hit
        int side = 0;           // Was the wall vertical or horizontal

        // Step direction and initial distance
        if (ray_vector.x < 0) {
            stepX = -1;
            sideDistX = (camera.xPos() - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - camera.xPos()) * deltaDistX;
        }
        if (ray_vector.y < 0) {
            stepY = -1;
            sideDistY = (camera.yPos() - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - camera.yPos()) * deltaDistY;
        }

        // Scan where ray hits a wall (DDA)
        while ( !hit ) {
            // Jump to next square
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            // Check for hit
            if (map_[mapX][mapY] != EMPTY_SPACE) {
                hit = true;
            }
        }

        // Calculate distance to the point of impact
        if (side == VERTICAL) {
            perpWallDist = (mapX - camera.xPos() + (1 - stepX) / 2) / ray_vector.x;
        } else {
            perpWallDist = (mapY - camera.yPos() + (1 - stepY) / 2) / ray_vector.y;
        }

        // Calculate height of the wall based on the instance
        int lineHeight = (int)(height_ / perpWallDist);

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + height_ / 2;
        if (drawStart < 0) {
            drawStart = 0;
        }
        int drawEnd = lineHeight / 2 + height_ / 2;
        if (drawEnd >= height_) {
            drawEnd = height_ - 1;
        }

        WallColor wall_color;
        switch(map_[mapX][mapY])
        {
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

        if (side == HORIZONTAL)
        {
            wall_color.r = (wall_color.r >> 1) & 8355711;
            wall_color.g = (wall_color.g >> 1) & 8355711;
            wall_color.b = (wall_color.b >> 1) & 8355711;
        }

        SDL_SetRenderDrawColor(renderer, wall_color.r, wall_color.g, wall_color.b, 255);
        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}
