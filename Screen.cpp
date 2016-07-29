#include "Screen.h"

#include <cmath>
#include "Camera.h"

/**
 * Calculate how the screen should look to the user based on their position on the map
 */
void Screen::update(const Camera& camera, SDL_Renderer* renderer) {


    // Cast rays
    for(int x = 0; x < width_; ++x)
    {
        double cameraX = 2 * x / (double)(width_) - 1;       // x coordinate of current vertical stripe of camera plane

        // Vectors for ray
        double rayDirX = camera.xDir() + camera.xPlane() * cameraX;
        double rayDirY = camera.yDir() + camera.yPlane() * cameraX;

        // Map position
        int mapX = (int)camera.xPos();
        int mapY = (int)camera.yPos();

        // Length of ry from current position to next x or y-side
        double sideDistX;
        double sideDistY;

        // Length of ray from one side to next in map
        double deltaDistX = std::sqrt( 1 + (rayDirY * rayDirY) / (rayDirX * rayDirX) );
        double deltaDistY = std::sqrt( 1 + (rayDirX * rayDirX) / (rayDirY * rayDirY) );

        double perpWallDist;    // Distance from player and the first wall
        int stepX, stepY;       // Direction to go in x and y
        bool hit = false;       // Was a wall hit
        int side = 0;           // Was the wall vertical or horizontal

        // Step direction and initial distance
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (camera.xPos() - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - camera.xPos()) * deltaDistX;
        }
        if (rayDirY < 0) {
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
            if (map_[mapX][mapY] > 0) {
                hit = true;
            }
        }

        // Calculate distance to the point of impact
        if (side == 0) {
            perpWallDist = (mapX - camera.xPos() + (1 - stepX) / 2) / rayDirX;
        } else {
            perpWallDist = (mapY - camera.yPos() + (1 - stepY) / 2) / rayDirY;
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

        if (side == 0)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 160, 0, 255);
        }

        SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
    }
}

