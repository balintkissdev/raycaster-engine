#pragma once

#include "Map.h"
#include "Vector2.h"

#include <cstdint>
#include <vector>

class Camera
{
public:
    static constexpr float DIRECTION_FORWARD = 1.f;
    static constexpr float DIRECTION_BACKWARD = -1.f;
    static constexpr float DIRECTION_LEFT = 1.f;
    static constexpr float DIRECTION_RIGHT = -1.f;

    Camera(const Vector2<float>& position, const Vector2<float>& direction, const float fieldOfView, Map& map);

    void move(const float moveDirection);
    void turn(const float turnDirection);
    void strafe(const float strafeDirection);

    [[nodiscard]] const Vector2<float>& position() const;

    /**
     * Direction where the camera points to.
     */
    [[nodiscard]] const Vector2<float>& direction() const;

    /**
     * Perpendicular to the direction vector and points to the farthest edge of the camera's field of view.
     */
    [[nodiscard]] const Vector2<float>& plane() const;

    Camera& movementSpeed(const float movementSpeed);
    Camera& rotationSpeed(const float rotationSpeed);

    [[nodiscard]] Vector2<float> planeLeftEdgeDirection() const;
    [[nodiscard]] Vector2<float> planeRightEdgeDirection() const;

private:
    Map& map_;
    Vector2<float> position_;
    Vector2<float> direction_;
    Vector2<float> plane_;
    float movementSpeed_, rotationSpeed_;
};

