#ifndef RAY_H
#define RAY_H

#include <cassert>
#include <iostream>
#include <Vector3d.h>


// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray {
public:

    Ray() = delete;
    Ray(const Vector3d &orig, const Vector3d &dir) {
        origin = orig;
        direction = dir;
    }

    Ray(const Ray &r) {
        origin = r.origin;
        direction = r.direction;
    }

    const Vector3d &getOrigin() const {
        return origin;
    }

    const Vector3d &getDirection() const {
        return direction;
    }

    Vector3d pointAtParameter(double t) const {
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

#endif // RAY_H
