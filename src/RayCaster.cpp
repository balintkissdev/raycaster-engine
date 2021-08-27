#include "RayCaster.h"

#include "Camera.h"
#include "IRenderer.h"
#include "Vector2.h"

#include <cmath>
#include <cstring>
#include <iostream>

RayCaster::RayCaster(Camera& camera, Map& map) : camera_(camera), map_(map) {}

bool RayCaster::init(IRenderer& renderer)
{
    screenWidth_ = renderer.screenWidth();
    screenHeight_ = renderer.screenHeight();
    drawBuffer_.resize(screenWidth_ * screenHeight_);

    topTexture_ = renderer.createTexture("resources/textures/dusk_sky_texture.bmp");
    bottomTexture_ = renderer.createTexture("resources/textures/floor.bmp");
    wallTextures_[0] = renderer.createTexture("resources/textures/wall0.bmp");
    wallTextures_[1] = renderer.createTexture("resources/textures/wall1.bmp");
    wallTextures_[2] = renderer.createTexture("resources/textures/wall2.bmp");
    wallTextures_[3] = renderer.createTexture("resources/textures/wall3.bmp");

    return topTexture_.has_value() && bottomTexture_.has_value() && wallTextures_[0].has_value() &&
           wallTextures_[1].has_value() && wallTextures_[2].has_value() && wallTextures_[3].has_value();
}

void RayCaster::drawEverything(IRenderer& renderer)
{
    drawTop();
    drawBottom();
    drawWalls();

    renderer.drawBuffer(drawBuffer_.data());
}

void RayCaster::drawTop()
{
    // FIXME: Use full sky texture, not just half. It bugs out if walls are too far.
    memcpy(drawBuffer_.data(), topTexture_->texels.data(), topTexture_->pitch * topTexture_->height);
}

void RayCaster::drawBottom()
{
    for (size_t y = screenHeight_ / 2 + 1; y < screenHeight_; ++y)
    {
        const Vector2<float> leftmostRayDirection = camera_.direction() - camera_.plane();
        const Vector2<float> rightmostRayDirection = camera_.direction() + camera_.plane();

        const size_t screenCenterDistance = y - screenHeight_ / 2;
        const float cameraVerticalPosition = 0.5f * static_cast<float>(screenHeight_);
        const float cameraToRowDistance = cameraVerticalPosition / static_cast<float>(screenCenterDistance);

        const Vector2<float> floorStep =
            cameraToRowDistance * (rightmostRayDirection - leftmostRayDirection) / static_cast<float>(screenWidth_);

        Vector2<float> floor = camera_.position() + (leftmostRayDirection * cameraToRowDistance);

        for (size_t x = 0; x < screenWidth_; ++x)
        {
            const Vector2<size_t> cell = {static_cast<size_t>(floor.x), static_cast<size_t>(floor.y)};

            const Vector2<size_t> texCoord = {
                static_cast<size_t>(
                    static_cast<float>(bottomTexture_->width) * (floor.x - static_cast<float>(cell.x))) &
                    (bottomTexture_->width - 1),
                static_cast<size_t>(
                    static_cast<float>(bottomTexture_->height) * (floor.y - static_cast<float>(cell.y))) &
                    (bottomTexture_->height - 1)};

            floor += floorStep;

            const uint32_t texel = bottomTexture_->texels[bottomTexture_->width * texCoord.y + texCoord.x];
            plotPixel(x, y, texel);
        }
    }
}

void RayCaster::drawWalls()
{
    // Cast rays horizontally
    for (size_t x = 0; x < screenWidth_; ++x)
    {
        // x coordinate of current vertical stripe of camera plane
        const float cameraPlaneVerticalStripeX = ((2 * static_cast<float>(x)) / static_cast<float>(screenWidth_)) - 1;

        const Vector2<float> rayDirection = camera_.direction() + (camera_.plane() * cameraPlaneVerticalStripeX);
        auto mapSquarePosition = static_cast<Vector2<int>>(camera_.position());

        // Length of ray from one side to next in map
        const Vector2<float> rayStepDistance = {
            sqrtf(1 + (rayDirection.y * rayDirection.y) / (rayDirection.x * rayDirection.x)),
            sqrtf(1 + (rayDirection.x * rayDirection.x) / (rayDirection.y * rayDirection.y))};

        auto [stepDirection, sideDistance] = calculateInitialStep(mapSquarePosition, rayDirection, rayStepDistance);
        const auto [side, mapSquareIndex] = performDDA(stepDirection, rayStepDistance, mapSquarePosition, sideDistance);

        const float wallDistance = calculateWallDistance(side, mapSquarePosition, stepDirection, rayDirection);

        const int wallColumnHeight = static_cast<int>(static_cast<float>(screenHeight_) / wallDistance);

        const auto [drawStart, drawEnd] = calculateDrawLocations(wallColumnHeight);

        if (mapSquareIndex != EMPTY_SPACE)
        {
            drawTexturedColumn(
                x, mapSquareIndex, side, wallDistance, wallColumnHeight, rayDirection, drawStart, drawEnd);
        }
    }
}

std::pair<Vector2<int>, Vector2<float>> RayCaster::calculateInitialStep(
    const Vector2<int>& mapSquarePosition, const Vector2<float>& rayDirection, const Vector2<float>& rayStepDistance)
{
    Vector2<int> stepDirection;   // Step direction and initial distance
    Vector2<float> sideDistance;  // Length of ry from current position to next x or y-side
    if (rayDirection.x < 0)
    {
        stepDirection.x = -1;
        sideDistance.x = (camera_.position().x - static_cast<float>(mapSquarePosition.x)) * rayStepDistance.x;
    }
    else
    {
        stepDirection.x = 1;
        sideDistance.x = (static_cast<float>(mapSquarePosition.x) + 1.0f - camera_.position().x) * rayStepDistance.x;
    }
    if (rayDirection.y < 0)
    {
        stepDirection.y = -1;
        sideDistance.y = (camera_.position().y - static_cast<float>(mapSquarePosition.y)) * rayStepDistance.y;
    }
    else
    {
        stepDirection.y = 1;
        sideDistance.y = (static_cast<float>(mapSquarePosition.y) + 1.0f - camera_.position().y) * rayStepDistance.y;
    }

    return {stepDirection, sideDistance};
}

std::pair<WallSide, size_t> RayCaster::performDDA(
    const Vector2<int>& stepDirection,
    const Vector2<float>& rayStepDistance,
    Vector2<int>& mapSquarePositionInOut,
    Vector2<float>& sideDistanceInOut) const
{
    // Scan where ray hits a wall
    WallSide side = VERTICAL;
    size_t mapSquareIndex = EMPTY_SPACE;
    while (true)
    {
        // Jump to next square
        if (sideDistanceInOut.x < sideDistanceInOut.y)
        {
            sideDistanceInOut.x += rayStepDistance.x;
            mapSquarePositionInOut.x += stepDirection.x;
            side = VERTICAL;
        }
        else
        {
            sideDistanceInOut.y += rayStepDistance.y;
            mapSquarePositionInOut.y += stepDirection.y;
            side = HORIZONTAL;
        }

        // Check for hit
        mapSquareIndex = map_.position(mapSquarePositionInOut.x, mapSquarePositionInOut.y);
        if (mapSquareIndex != EMPTY_SPACE)
        {
            break;
        }
    }

    return {side, mapSquareIndex};
}

float RayCaster::calculateWallDistance(
    const WallSide side,
    const Vector2<int>& mapSquarePosition,
    const Vector2<int>& stepDirection,
    const Vector2<float>& rayDirection)
{
    return (side == VERTICAL) ? ((static_cast<float>(mapSquarePosition.x) - camera_.position().x +
                                  static_cast<float>(1 - stepDirection.x) / 2) /
                                 rayDirection.x)
                              : ((static_cast<float>(mapSquarePosition.y) - camera_.position().y +
                                  static_cast<float>(1 - stepDirection.y) / 2) /
                                 rayDirection.y);
}

std::pair<int, int> RayCaster::calculateDrawLocations(const int wallColumnHeight) const
{
    int drawStart = -wallColumnHeight / 2 + screenHeight_ / 2;
    if (drawStart < 0)
    {
        drawStart = 0;
    }

    int drawEnd = wallColumnHeight / 2 + screenHeight_ / 2;
    if (drawEnd >= screenHeight_)
    {
        drawEnd = screenHeight_ - 1;
    }

    return {drawStart, drawEnd};
}

void RayCaster::drawTexturedColumn(
    const size_t x,
    const size_t mapSquareIndex,
    const WallSide side,
    const float wallDistance,
    const int wallColumnHeight,
    Vector2<float> ray,
    const int drawStart,
    const int drawEnd)
{
    // 1 subtracted from it so that texture 0 can be used
    const Texture& wallTexture = *(wallTextures_[mapSquareIndex - 1]);

    float wallHitX;
    if (side == VERTICAL)
    {
        wallHitX = camera_.position().y + wallDistance * ray.y;
    }
    else
    {
        wallHitX = camera_.position().x + wallDistance * ray.x;
    }
    wallHitX -= std::floor((wallHitX));

    auto texCoordX = static_cast<size_t>(wallHitX * static_cast<float>(wallTexture.width));
    if (side == VERTICAL && ray.x > 0)
    {
        texCoordX = wallTexture.width - texCoordX - 1;
    }
    if (side == HORIZONTAL && ray.y < 0)
    {
        texCoordX = wallTexture.width - texCoordX - 1;
    }

    // How much to increase the texture coordinate per screen pixel
    const float texCoordIncreaseStep =
        1.0f * static_cast<float>(wallTexture.height) / static_cast<float>(wallColumnHeight);
    float startingTexCoordY = (static_cast<float>(drawStart) - static_cast<float>(screenHeight_) / 2 +
                               static_cast<float>(wallColumnHeight) / 2) *
                              texCoordIncreaseStep;
    for (int y = drawStart; y < drawEnd; ++y)
    {
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of
        // overflow
        const size_t texCoordY = static_cast<size_t>(startingTexCoordY) & (wallTexture.height - 1);
        startingTexCoordY += texCoordIncreaseStep;
        const size_t texelIndex = wallTexture.height * texCoordY + texCoordX;
        uint32_t texel = wallTexture.texels[texelIndex];

        // Shade horizontal sides darker
        if (side == HORIZONTAL)
        {
            texel = (texel >> 1) & DARKEN_MASK;
        }

        plotPixel(x, y, texel);
    }
}

void RayCaster::plotPixel(const uint16_t x, const uint16_t y, const uint32_t pixel)
{
    drawBuffer_[y * screenWidth_ + x] = pixel;
}