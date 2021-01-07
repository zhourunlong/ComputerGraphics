#pragma once

#include <bits/stdc++.h>
#include "ray.hpp"
#include "vecmath/vecmath.h"
#include "sampler.hpp"

class Camera {
public:
    Camera() {
        center = Vector3d::ZERO;
        target = Vector3d(0, 0, 1);
        up = Vector3d(0, 1, 0);
        fovAxis = "x";
        aperture = 0;
        distance = 1;
    }

    inline void setAngle(const double &_angle) {angle = _angle;}

    inline void setFovAxis(const std::string &_foxAxis) {fovAxis = _foxAxis;}

    inline void setAperture(const double &_aperture) {
        aperture = _aperture;
    }

    inline void setDistance(const double &_distance) {
        distance = _distance;
    }

    inline Vector3d getCenter() {return center;}

    inline void setCenter(const Vector3d &_center) {center = _center;}

    inline void setTarget(const Vector3d &_target) {target = _target;}

    inline void setUp(const Vector3d &_up) {up = _up;}

    inline void setWidth(const int &_width) {width = _width;}
    inline int getWidth() {return width;}

    inline void setHeight(const int &_height) {height = _height;}
    inline int getHeight() {return height;}

    inline void finish() {
        direction = (target - center).normalized();
        up = (up - center).normalized();
        horizontal = Vector3d::cross(direction, up);
    }

    inline Ray generateRay(const Vector2d &point, Sampler* sampler) {
        Vector3d dir;

        if (fovAxis == "y")
            dir = tan(angle / 2) * (point.x() / height * 2 - 1.0 * width / height) * horizontal
                + tan(angle / 2) * (point.y() / height * 2 - 1) * up
                + direction;
        else if (fovAxis == "x")
            dir = tan(angle / 2) * (point.y() / width * 2 - 1.0 * height / width) * up
                + tan(angle / 2) * (point.x() / width * 2 - 1) * horizontal
                + direction;
        
        double t = distance / Vector3d::dot(dir, direction);
        Vector3d x = center + t * dir;
        Vector2d dc = sampler->sampleDiskUniform();
        Vector3d nc = center + aperture * dc.x() * up + aperture * dc.y() * horizontal;
        
        return Ray(nc, (x - nc).normalized());
    }

    inline void print() {
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
    double angle, aperture, distance;
    std::string fovAxis;
};
