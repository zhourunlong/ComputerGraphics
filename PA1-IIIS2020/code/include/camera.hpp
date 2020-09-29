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
        this->up = up.normalized();
        this->horizontal = Vector3f::cross(this->direction, this->up);
        //this->up = Vector3f::cross(this->horizontal, this->direction);
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
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        cx = imgW / 2.0; cy = imgH / 2.0;
        d = cy / tan(angle / 2);
        d = sqrt(d * d - cx * cx);
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f dir = (point.x() - cx) * horizontal + (point.y() - cy) * up + d * direction; // fx = fy = d
        return Ray(center, dir);
    }

protected:
    float cx, cy, d; // d is the distance from camera to pic plane
};

#endif //CAMERA_H
