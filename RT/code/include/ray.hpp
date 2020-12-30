#pragma once

#include <cassert>
#include <iostream>
#include "vecmath/vecmath.h"

class Ray {
public:

    Ray() = delete;
    inline Ray(const Vector3d &orig, const Vector3d &dir) {
        origin = orig;
        direction = dir;
    }

    inline Ray(const Ray &r) {
        origin = r.origin;
        direction = r.direction;
    }

    inline const Vector3d &getOrigin() const {return origin;}

    inline void setOrigin(const Vector3d &_origin) {origin = _origin;}

    inline const Vector3d &getDirection() const {return direction;}

    inline Vector3d pointAtParameter(double t) const {
        return origin + direction * t;
    }

private:

    Vector3d origin;
    Vector3d direction;

};

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
    return os;
}
