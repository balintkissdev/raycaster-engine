#include "RayCaster.h"

#include "Camera.h"

const int TEXTURE_SIZE = 128;

using namespace mymath;

void RayCaster::drawCeilingAndFloor(SDL_Renderer* renderer)
{
    // "Ceiling"
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_Rect rect = {0, 0, width_, height_/ 2};
    SDL_RenderFillRect(renderer, &rect);

    // "Floor"
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    rect = {0, height_ / 2, width_, height_};
    SDL_RenderFillRect(renderer, &rect);
};

void RayCaster::drawCeilingAndFloor(SDL_Renderer* renderer, SDL_Texture* top_texture)
{
    // Top
    SDL_RenderCopy(renderer, top_texture, nullptr, nullptr);

    // Bottom   (TODO)
    SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
    SDL_Rect rect = {0, height_ / 2, width_, height_};
    SDL_RenderFillRect(renderer, &rect);
};

/**
 * Calculate how the screen should look to the user based on their position on the map
 */
void RayCaster::drawWalls(SDL_Renderer* renderer, 
        const Camera& camera,
        const std::vector< std::shared_ptr<SDL_Surface> >& wall_textures)
{

    // Cast rays horizontally
    for (int x = 0; x < width_; ++x)
    {
        // x coordinate of current vertical stripe of camera plane
        double camera_x = ((2 * x) / static_cast<double>(width_)) - 1;       

        // Vectors for ray
        Vector2d<double> ray_vector(camera.direction().x + camera.plane().x * camera_x, 
                                            camera.direction().y + camera.plane().y * camera_x);

        // Map position
        Point2d<int> square_on_map(static_cast<int>(camera.position().x), 
                                   static_cast<int>(camera.position().y));

        // Length of ray from one side to next in map
        // TODO: Cleanup
        double delta_distance_x = sqrt( 1 + (ray_vector.y * ray_vector.y) / (ray_vector.x * ray_vector.x) );
        double delta_distance_y = sqrt( 1 + (ray_vector.x * ray_vector.x) / (ray_vector.y * ray_vector.y) );

        // Length of ry from current position to next x or y-side
        double side_dist_x, side_dist_y;

        // Step direction and initial distance
        int step_x, step_y;                 // Direction to go in x and y
        if (ray_vector.x < 0)
        {
            step_x = -1;
            side_dist_x = (camera.position().x - square_on_map.x) * delta_distance_x;
        }
        else
        {
            step_x = 1;
            side_dist_x = (square_on_map.x + 1.0 - camera.position().x) * delta_distance_x;
        }
        if (ray_vector.y < 0)
        {
            step_y = -1;
            side_dist_y = (camera.position().y - square_on_map.y) * delta_distance_y;
        }
        else
        {
            step_y = 1;
            side_dist_y = (square_on_map.y + 1.0 - camera.position().y) * delta_distance_y;
        }

        // Scan where ray hits a wall (DDA)
        int side = VERTICAL;                // Was the wall vertical or horizontal
        bool is_wall_hit = false;
        while (!is_wall_hit)
        {
            // Jump to next square
            if (side_dist_x < side_dist_y)
            {
                side_dist_x += delta_distance_x;
                square_on_map.x += step_x;
                side = VERTICAL;
            }
            else
            {
                side_dist_y += delta_distance_y;
                square_on_map.y += step_y;
                side = HORIZONTAL;
            }

            // Check for hit
            if (map_[square_on_map.x][square_on_map.y] != EMPTY_SPACE) {
                is_wall_hit = true;
            }
        }

        // Calculate distance to the point of impact
        double wall_to_player_distance;     // Distance from player and the first wall
        if (side == VERTICAL)
        {
            wall_to_player_distance = (square_on_map.x - camera.position().x + (1 - step_x) / 2) / ray_vector.x;
        }
        else
        {
            wall_to_player_distance = (square_on_map.y - camera.position().y + (1 - step_y) / 2) / ray_vector.y;
        }

        // Calculate height of the wall
        int wall_stripe_height = static_cast<int>(height_ / wall_to_player_distance);

        // Calculate lowest and highest pixel to fill in current stripe
        int draw_start = -wall_stripe_height / 2 + height_ / 2;
        if (draw_start < 0)
        {
            draw_start = 0;
        }
        int draw_end = wall_stripe_height / 2 + height_ / 2;
        if (draw_end >= height_)
        {
            draw_end = height_ - 1;
        }

        drawPlainColoredStripe(renderer, x, square_on_map, draw_start, draw_end, side);

        // Create Cel Shader-like effect for walls
        // HACK: drawing over already drawn stripe is an unoptimalized way, needs work
        const int shade_stroke = 5;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, x, draw_start, x, draw_start + shade_stroke);
        SDL_RenderDrawLine(renderer, x, draw_end - shade_stroke, x, draw_end);
    }
}

inline void RayCaster::drawPlainColoredStripe(SDL_Renderer* renderer, const int x, const mymath::Point2d<int>& square_on_map, const int draw_start, const int draw_end, const int side)
{
    WallColor wall_color;
    switch(map_[square_on_map.x][square_on_map.y])
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
    SDL_RenderDrawLine(renderer, x, draw_start, x, draw_end);
}

// FIXME: unoptimized, slow
WallColor RayCaster::extractPixelColor(SDL_Surface* wall, const mymath::Point2d<int>& pixel_position)
{
    SDL_LockSurface(wall);

    Uint32* pixel_array = static_cast<Uint32*>(wall->pixels);
    Uint32 pixel = pixel_array[(pixel_position.y * wall->w) + pixel_position.x];
    WallColor color;
    SDL_GetRGB(pixel, wall->format, &color.r, &color.g, &color.b);

    SDL_UnlockSurface(wall);

    return color;
}
