#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        center = Vector3d(0, 0, 0);
        radius = 1;
    }

    Sphere(const Vector3d &center, double radius, Material *material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
    }

    void setRadius(const double &_radius) {radius = _radius;}

    void setCenter(const Vector3d &_center) {center = _center;}

    bool intersect(const Ray &r, Hit &h, double tmin) override {
        Vector3d p = r.getOrigin(), v = r.getDirection();
        Vector3d pc = center - p;

        // disjointed
        double d = Vector3d::cross(pc, v).length();
        if (d >= radius) return false;

        double PC = pc.length();
        double t = sqrt(PC * PC - d * d), s = sqrt(radius * radius - d * d);
        bool into;
        Vector3d n;
        // inside sphere
        if (PC <= radius) {
            t += s;
            n = center - r.pointAtParameter(t);
            into = false;
        } else {
        // outside
            // wrong direction
            if (Vector3d::dot(v, pc) < 0) return false;

            // correct direction
            t -= s;
            n = r.pointAtParameter(t) - center;
            into = true;
        }

        if (t >= tmin && t < h.getT()) {
            h.set(t, this, n, into);
            return true;
        }
        return false;
    }

    BoundPlane getBoundPlaneX() override {
        return (BoundPlane){center.x() - radius, center.x() + radius};
    }

    BoundPlane getBoundPlaneY() override {
        return (BoundPlane){center.y() - radius, center.y() + radius};
    }

    BoundPlane getBoundPlaneZ() override {
        return (BoundPlane){center.z() - radius, center.z() + radius};
    }

    void print() override {
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