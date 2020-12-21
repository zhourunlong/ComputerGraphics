#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include "vecmath/vecmath.h"
#include <cmath>

class Sphere : public Object3D {
public:
    inline Sphere() {
        // unit ball at the center
        center = Vector3d(0, 0, 0);
        radius = 1;
    }

    inline Sphere(const Vector3d &center, double radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
    }

    inline void setRadius(const double &_radius) {radius = _radius;}

    inline void setCenter(const Vector3d &_center) {center = _center;}

    inline bool intersect(const Ray &r, Hit &h, double tmin) override {
        Vector3d v = r.getDirection().normalized(), pc = r.getOrigin() - center;
        double b = Vector3d::dot(v, pc), c = pc.squaredLength() - radius * radius;
        double det = b * b - c;
        if (det < 0) return false;
        det = sqrt(det);
        double t, th = tmin * r.getDirection().length();
        if (b > 0) {
            t = -b - det;
            if (t < th) t = c / t;
        } else {
            t = c / (det - b);
            if (t < th) t = det - b;
        }
        //fprintf(stderr, "(b=%lf t=%lf)", b, t);
        t /= r.getDirection().length();
        if (t >= tmin && t < h.getT()) {
            Vector3d n = center - r.pointAtParameter(t);
            if (Vector3d::dot(n, v) < 0)
                h.set(t, this, n, false);
            else
                h.set(t, this, -n, true);
            return true;
        }
        return false;
    }

    inline BoundPlane getBoundPlaneX() override {
        return (BoundPlane){center.x() - radius, center.x() + radius};
    }

    inline BoundPlane getBoundPlaneY() override {
        return (BoundPlane){center.y() - radius, center.y() + radius};
    }

    inline BoundPlane getBoundPlaneZ() override {
        return (BoundPlane){center.z() - radius, center.z() + radius};
    }

    inline void print() override {
        std::cout << "===== Sphere =====\n";
        std::cout << "center: " << center << "\n";
        std::cout << "radius: " << radius << "\n";
        std::cout << "material: " << ref << "\n";
        material->print();
        if (emmision != Vector3d::ZERO)
            std::cout << "emmision: " << emmision << "\n";
        std::cout << "------------------\n";
    }

protected:
    Vector3d center;
    double radius;
};


#endif
