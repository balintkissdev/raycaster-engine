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

double Camera::xPos() const { return position_.x; }
double Camera::yPos() const { return position_.y; }
double Camera::xDir() const { return direction_vector_.x; }
double Camera::yDir() const { return direction_vector_.y; }
double Camera::xPlane() const { return plane_vector_.x; }
double Camera::yPlane() const { return plane_vector_.y; }

void Camera::movSpeed(double mov_speed) { movement_speed_ = mov_speed; }
void Camera::rotSpeed(double rot_speed) { rotation_speed_ = rot_speed; }
