#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "MathLib.h"

class Camera
{
    public:
        Camera(const double x, const double y, 
                const double xd, const double yd,
                const double xp, const double yp,
                const std::vector< std::vector<int> >& map)
            : position_(x, y)
            , direction_vector_{xd, yd}
            , plane_vector_{xp, yp}
            , movement_speed_(0.0), rotation_speed_(0.0)
            , map_(map)
            {}

        /**
         * CONTROLS
         */
        void moveForward();
        void moveBackward();
        void turnLeft();
        void turnRight();
        void strafeLeft();
        void strafeRight();

        /**
         * GETTERS
         */
        const mymath::Vector2d<double>& position() const;
        const mymath::Vector2d<double>& direction() const;
        const mymath::Vector2d<double>& plane() const;

        /**
         * SETTERS
         */
        Camera& movementSpeed(double mov_speed);
        Camera& rotationSpeed(double rot_speed);

    private:
        mymath::Vector2d<double> position_;
        mymath::Vector2d<double> direction_vector_;
        mymath::Vector2d<double> plane_vector_;
        double movement_speed_, rotation_speed_;
        const std::vector< std::vector<int> >& map_;       // TODO: change to pointer
};

#endif
