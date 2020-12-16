#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>

class Camera {
public:
    Camera(const Vector3f &center = Vector3f::ZERO, const Vector3f &direction = Vector3f(0, 0, 1),
        const Vector3f &up = Vector3f(0, 1, 0), const int imgW = 1920, const int imgH = 1080, const float angle = 30) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
        this->angle = angle;
    }

    void setAngle(const float &_angle) {angle = _angle;}

    void setCenter(const Vector3f &_center) {center = _center;}

    void setTarget(const Vector3f &_target) {target = _target;}

    void setUp(const Vector3f &_up) {up = _up.normalized();}

    void setWidth(const int &_width) {width = _width;}

    void setHeight(const int &_height) {height = _height;}

    void finish() {
        direction = (target - center).normalized();
        horizontal = Vector3f::cross(direction, up);
    }

    Ray generateRay(const Vector2f &point) {
        Vector3f dir = tan(angle / 2) * (point.x() / width * 2 - 1) * horizontal +
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
    Vector3f center, direction, target, up, horizontal;
    int width, height;
    float angle;
};

#endif //CAMERA_H

