#include "Camera.h"

void Camera::moveForward()
{
    if (map_[static_cast<int>(position_.x + direction_vector_.x * movement_speed_)]
            [static_cast<int>(position_.y)] == 0)
    {
        position_.x += direction_vector_.x * movement_speed_;
    }
    if (map_[static_cast<int>(position_.x)]
            [static_cast<int>(position_.y + direction_vector_.y * movement_speed_)] == 0 )
    {
        position_.y += direction_vector_.y * movement_speed_;
    }
}

// FIXME: boilerplate code
void Camera::moveBackward()
{
    if (map_[static_cast<int>(position_.x - direction_vector_.x * movement_speed_)]
            [static_cast<int>(position_.y)] == 0 )
    {
        position_.x -= direction_vector_.x * movement_speed_;
    }

    if (map_[static_cast<int>(position_.x)]
            [static_cast<int>(position_.y - direction_vector_.y * movement_speed_)] == 0 )
    {
        position_.y -= direction_vector_.y * movement_speed_;
    }
}

// Rotation: direction and plane vectors are multiplied by rotation matrix
// Rotation matrix is:
//      [ cos(ROTATION_SPEED) -sin(ROTATION_SPEED) ]
//      [ sin(ROTATION_SPEED) cos(ROTATION_SPEED) ]
void Camera::turnLeft()
{
    direction_vector_ = mymath::rotate(direction_vector_, rotation_speed_);
    plane_vector_ = mymath::rotate(plane_vector_, rotation_speed_);
}

void Camera::turnRight()
{
    direction_vector_ = mymath::rotate(direction_vector_, -rotation_speed_);
    plane_vector_ = mymath::rotate(plane_vector_, -rotation_speed_);
}

void Camera::strafeLeft()
{
    if (map_[static_cast<int>(position_.x - plane_vector_.x * movement_speed_)]
            [static_cast<int>(position_.y)] == 0)
    {
        position_.x -= plane_vector_.x * movement_speed_;
    }
    if (map_[static_cast<int>(position_.x)]
            [static_cast<int>(position_.y - plane_vector_.y * movement_speed_)] == 0 )
    {
        position_.y -= plane_vector_.y * movement_speed_;
    }
}

void Camera::strafeRight()
{
    if (map_[static_cast<int>(position_.x + plane_vector_.x * movement_speed_)]
            [static_cast<int>(position_.y)] == 0 )
    {
        position_.x += plane_vector_.x * movement_speed_;
    }

    if (map_[static_cast<int>(position_.x)]
            [static_cast<int>(position_.y + plane_vector_.y * movement_speed_)] == 0 )
    {
        position_.y += plane_vector_.y * movement_speed_;
    }
}

const mymath::Vector2d<double>& Camera::position() const { return position_; }
const mymath::Vector2d<double>& Camera::direction() const { return direction_vector_; }
const mymath::Vector2d<double>& Camera::plane() const { return plane_vector_; }

Camera& Camera::movementSpeed(double mov_speed) { movement_speed_ = mov_speed; return *this; }
Camera& Camera::rotationSpeed(double rot_speed) { rotation_speed_ = rot_speed; return *this; }
