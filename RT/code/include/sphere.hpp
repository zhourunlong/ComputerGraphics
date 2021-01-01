#pragma once

#include "object3d.hpp"
#include "vecmath/vecmath.h"
#include <cmath>

class Sphere : public Object3D {
public:
    inline Sphere() {
        objType = SPHERE;
        center = Vector3d(0, 0, 0);
        radius = 1;
    }

    inline Sphere(const Vector3d &center, double radius, Material *material) : Object3D(material) {
        objType = SPHERE;
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

    inline bool getSample(const Vector3d &x, Vector3d &y, Vector3d &ny, double &A, unsigned short *Xi) override {
        Vector3d tz = x - center;
        double l = tz.length();
        if (l <= radius) return false;
        double cosThMx = radius / l;
        A = 2 * M_PI * radius * radius * (1 - cosThMx);
        double theta = acos(cosThMx + erand48(Xi) * (1 - cosThMx)), phi = 2 * M_PI * erand48(Xi);
        tz.normalize();
        Vector3d tx = Vector3d::cross(tz, (fabs(tz.x()) > 0.1) ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0)).normalized();
        Vector3d ty = Vector3d::cross(tz, tx);
        ny = sin(theta) * cos(phi) * tx + sin(theta) * sin(phi) * ty + cos(theta) * tz;
        y = center + radius * ny;
        return true;
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
