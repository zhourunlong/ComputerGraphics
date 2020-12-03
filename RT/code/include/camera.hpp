#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
        const Vector3f &up, int imgW, int imgH, float angle)
        : Camera(center, direction, up, imgW, imgH), angle(angle) {
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f dir = tan(angle / 2) * (point.x() / width * 2 - 1) * horizontal +
                       tan(angle / 2) * (point.y() / height * 2 - 1) * up +
                       direction;
        return Ray(this->center, dir.normalized());
    }

protected:
    float angle;
};

#endif //CAMERA_H

