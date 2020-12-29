#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "vecmath/vecmath.h"
#include <bits/stdc++.h>

class Camera {
public:
    Camera() {
        center = Vector3d::ZERO;
        target = Vector3d(0, 0, 1);
        up = Vector3d(0, 1, 0);
    }

    void setAngle(const double &_angle) {angle = _angle;}

    Vector3d getCenter() {return center;}

    void setCenter(const Vector3d &_center) {center = _center;}

    void setTarget(const Vector3d &_target) {target = _target;}

    void setUp(const Vector3d &_up) {up = _up;}

    void setWidth(const int &_width) {width = _width;}

    void setHeight(const int &_height) {height = _height;}

    void finish() {
        direction = (target - center).normalized();
        up = (up - center).normalized();
        horizontal = Vector3d::cross(direction, up);
    }

    Ray generateRay(const Vector2d &point) {
        Vector3d dir = tan(angle / 2) * (point.x() / height * 2 - 1.0 * width / height) * horizontal +
                       tan(angle / 2) * (point.y() / height * 2 - 1) * up +
                       direction;
        return Ray(this->center, dir.normalized());
    }

    int getWidth() {return width;}

    int getHeight() {return height;}

    void print() {
        std::cout << "===== Camera =====\n";
        std::cout << "center: " << center << "\n";
        std::cout << "direction: " << direction << "\n";
        std::cout << "up: " << up << "\n";
        std::cout << "horizontal: " << horizontal << "\n";
        std::cout << "resolution: " << width << " * " << height << "\n";
        std::cout << "angle: " << angle << "\n";
        std::cout << "------------------\n";
    }

protected:
    Vector3d center, direction, target, up, horizontal;
    int width, height;
    double angle;
};

#endif //CAMERA_H

