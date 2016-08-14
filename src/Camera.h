#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include <SDL2/SDL.h>

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

        void update(const std::vector< std::vector<int> >& map);

        // TODO: cleanup
        double xPos() const;
        double yPos() const;
        double xDir() const;
        double yDir() const;
        double xPlane() const;
        double yPlane() const;

        void movSpeed(double mov_speed);
        void rotSpeed(double rot_speed);

        void moveForward();
        void moveBackward();
        void turnLeft();
        void turnRight();
        void strafeLeft();
        void strafeRight();

    private:
        mymath::Point2d<double> position_;
        mymath::Vector2d<double> direction_vector_;
        mymath::Vector2d<double> plane_vector_;
        double movement_speed_, rotation_speed_;
        std::vector< std::vector<int> > map_;
};

#endif
