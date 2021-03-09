#include "RayCaster.h"

#include "Camera.h"
#include "IRenderer.h"

#include <cstring>
#include <iostream>

uint32_t RayCaster::drawBuffer[768][1024] = {0};
const uint8_t RayCaster::texture_width = 64;
const uint8_t RayCaster::texture_height = 64;

RayCaster::RayCaster(
    const std::vector<std::vector<int>> &map,
    const int width,
    const int height
    )
    : map_(map), width_(width), height_(height)
{
}

bool RayCaster::initialize(IRenderer *renderer)
{
    bool success = renderer->createTexture(sky_texture_, 1024, 348, "resources/textures/dusk_sky_texture.bmp") &&
        renderer->createTexture(wall_textures_[0], texture_width, texture_height, "resources/textures/wall0.bmp") &&
        renderer->createTexture(wall_textures_[1], texture_width, texture_height, "resources/textures/wall1.bmp") &&
        renderer->createTexture(wall_textures_[2], texture_width, texture_height, "resources/textures/wall2.bmp") &&
        renderer->createTexture(wall_textures_[3], texture_width, texture_height, "resources/textures/wall3.bmp");

    return success;
}

void RayCaster::drawTop(IRenderer *renderer)
{
    // HACK: Draw sky to buffer. Figure out copying hardware texture to other texture
    memcpy(drawBuffer, sky_texture_.pixels.data(), sky_texture_.pitch * sky_texture_.height);
}

void RayCaster::drawBottom(IRenderer *renderer)
{
    // HACK: Draw grey floor to buffer
    for (int x = 0; x < width_; ++x)
    {
        for (int y = (height_ / 2) + 1; y < height_; ++y)
        {
            drawBuffer[y][x] = (160 << 16) | (160 << 8) | 160;
        }
    }
}

/**
 * Calculate how the screen should look to the user based on their position on the map
 */
// TODO: Very long function
void RayCaster::drawWalls(IRenderer *renderer, const Camera &camera)
{
    // Cast rays horizontally
    for (int x = 0; x < width_; ++x)
    {
        // x coordinate of current vertical stripe of camera plane
        double camera_x = ((2 * x) / static_cast<double>(width_)) - 1;

        // Vectors for ray
        mymath::Vector2d<double> ray_vector(camera.direction().x + camera.plane().x * camera_x,
                                            camera.direction().y + camera.plane().y * camera_x);

        // Map position
        mymath::Point2d<int> square_on_map(static_cast<int>(camera.position().x),
                                           static_cast<int>(camera.position().y));

        // Length of ray from one side to next in map
        // TODO: Cleanup
        double delta_distance_x = sqrt(1 + (ray_vector.y * ray_vector.y) / (ray_vector.x * ray_vector.x));
        double delta_distance_y = sqrt(1 + (ray_vector.x * ray_vector.x) / (ray_vector.y * ray_vector.y));

        // Length of ry from current position to next x or y-side
        double side_dist_x, side_dist_y;

        // Step direction and initial distance
        int step_x, step_y; // Direction to go in x and y
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
        int side = VERTICAL; // Was the wall vertical or horizontal
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
            if (map_[square_on_map.x][square_on_map.y] != EMPTY_SPACE)
            {
                is_wall_hit = true;
            }
        }

        // Calculate distance to the point of impact
        double wall_to_player_distance; // Distance from player and the first wall
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

      // Texturing calculations
      if (map_[square_on_map.x][square_on_map.y] != EMPTY_SPACE)
      {

        int textureIndex = map_[square_on_map.x][square_on_map.y] - 1; // 1 subtracted from it so that texture 0 can be used

        double wall_hit_x; // Where exactly the wall was hit
        if(side == VERTICAL)
        {
            wall_hit_x = camera.position().y + wall_to_player_distance * ray_vector.y;
        }
        else
        {
            wall_hit_x = camera.position().x + wall_to_player_distance * ray_vector.x;
        }
        wall_hit_x -= floor((wall_hit_x));

        int texture_x_coordinate = int(wall_hit_x * double(texture_width));
        if(side == VERTICAL && ray_vector.x  > 0)
        {
            texture_x_coordinate = texture_width - texture_x_coordinate - 1;
        }
        if(side == HORIZONTAL && ray_vector.y < 0)
        {
            texture_x_coordinate = texture_width - texture_x_coordinate - 1;
        }

        // How much to increase the texture coordinate per screen pixel
        double step = 1.0 * texture_height / wall_stripe_height;
        // Starting texture coordinate
        double texPos = (draw_start - height_ / 2 + wall_stripe_height / 2) * step;
        for(int y = draw_start; y < draw_end; y++)
        {
            // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
            int texY = (int)texPos & (texture_height - 1);
            texPos += step;
            uint32_t color = wall_textures_[textureIndex].pixels[texture_height * texY + texture_x_coordinate];

            // Shade horizontal sides darker
            if(side == HORIZONTAL)
            {
                color = (color >> 1) & 8355711;
            }

            drawBuffer[y][x] = color;
        }
      }
    }

    renderer->drawBuffer(drawBuffer[0]);
}
