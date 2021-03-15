#include "Camera.h"

#include "Matrix2.h"
#include "WallTypes.h"

Camera::Camera(const Vector2<float>& position, const Vector2<float>& direction, const float fieldOfView, Map& map)
    : map_(map)
    , position_(position)
    , direction_(direction)
    , plane_{0.f, -0.01f * fieldOfView}
    , movementSpeed_(0.f)
    , rotationSpeed_(0.f)
{
}

void Camera::move(const float moveDirection)
{
    if (map_.position(
            static_cast<int>(position_.x + moveDirection * (direction_.x * movementSpeed_)),
            static_cast<int>(position_.y)) == EMPTY_SPACE)
    {
        position_.x += moveDirection * (direction_.x * movementSpeed_);
    }
    if (map_.position(
            static_cast<int>(position_.x),
            static_cast<int>(position_.y + moveDirection * (direction_.y * movementSpeed_))) == EMPTY_SPACE)
    {
        position_.y += moveDirection * (direction_.y * movementSpeed_);
    }
}

void Camera::turn(const float turnDirection)
{
    direction_ = Matrix::rotate(direction_, turnDirection * rotationSpeed_);
    plane_ = Matrix::rotate(plane_, turnDirection * rotationSpeed_);
}

void Camera::strafe(const float strafeDirection)
{
    if (map_.position(
            static_cast<int>(position_.x - strafeDirection * (plane_.x * movementSpeed_)),
            static_cast<int>(position_.y)) == EMPTY_SPACE)
    {
        position_.x -= strafeDirection * (plane_.x * movementSpeed_);
    }
    if (map_.position(
            static_cast<int>(position_.x),
            static_cast<int>(position_.y - strafeDirection * (plane_.y * movementSpeed_))) == EMPTY_SPACE)
    {
        position_.y -= strafeDirection * (plane_.y * movementSpeed_);
    }
}

const Vector2<float>& Camera::position() const
{
    return position_;
}

const Vector2<float>& Camera::direction() const
{
    return direction_;
}

const Vector2<float>& Camera::plane() const
{
    return plane_;
}

Camera& Camera::movementSpeed(const float movementSpeed)
{
    movementSpeed_ = movementSpeed;
    return *this;
}

Camera& Camera::rotationSpeed(const float rotationSpeed)
{
    rotationSpeed_ = rotationSpeed;
    return *this;
}
