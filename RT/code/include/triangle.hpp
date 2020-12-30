#pragma once

#include "object3d.hpp"
#include "vecmath/vecmath.h"
#include <cmath>
#include <iostream>
using namespace std;

class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
    inline Triangle(const Vector3d& a, const Vector3d& b, const Vector3d& c)
        : a(a), b(b), c(c) {
        calc();
	}

	inline Triangle(const Vector3d& a, const Vector3d& b, const Vector3d& c, Material* m)
        : Object3D(m), a(a), b(b), c(c) {
        calc();
	}

    inline void calc() {
        normal = Vector3d::cross(b - a, c - a);
        size = normal.length();
        normal.normalize();
        d = Vector3d::dot(normal, a);
        
        double Min = a.x(), Max = a.x();
        if (b.x() < Min) Min = b.x();
        else if (b.x() > Max) Max = b.x();
        if (c.x() < Min) Min = c.x();
        else if (c.x() > Max) Max = c.x();
        planeX = (BoundPlane){Min, Max};

        Min = Max = a.y();
        if (b.y() < Min) Min = b.y();
        else if (b.y() > Max) Max = b.y();
        if (c.y() < Min) Min = c.y();
        else if (c.y() > Max) Max = c.y();
        planeY = (BoundPlane){Min, Max};

        Min = Max = a.z();
        if (b.z() < Min) Min = b.z();
        else if (b.z() > Max) Max = b.z();
        if (c.z() < Min) Min = c.z();
        else if (c.z() > Max) Max = c.z();
        planeZ = (BoundPlane){Min, Max};
    }

	inline bool intersect(const Ray& r,  Hit& h, double tmin) override {
        Vector3d p = r.getOrigin(), v = r.getDirection();

        // parallel 
        double nv = Vector3d::dot(normal, v);
        if (nv == 0) return false;

        double t = (d - Vector3d::dot(normal, p)) / nv;

        // discard point out of range
        if (t < tmin || t >= h.getT()) return false;

        Vector3d q = r.pointAtParameter(t);

        // Barycentric coordinates
        double alpha = Vector3d::dot(normal, Vector3d::cross(c - b, q - b));
        if (alpha < 0 || alpha > size) return false;

        double beta = Vector3d::dot(normal, Vector3d::cross(q - a, c - a));
        if (beta >= 0 && alpha + beta <= size) {
            if (Vector3d::dot(v, normal) < 0)
                h.set(t, this, normal, true);
            else 
                h.set(t, this, -normal, false);
            return true;
        }

        return false;
	}

    inline BoundPlane getBoundPlaneX() override {return planeX;}
    inline BoundPlane getBoundPlaneY() override {return planeY;}
    inline BoundPlane getBoundPlaneZ() override {return planeZ;}

    inline void print() override {
        std::cout << "===== Triangle =====\n";
        std::cout << a << "\n" << b << "\n" << c << "\n";
        std::cout << "material: " << ref << "\n";
        material->print();
        if (emmision != Vector3d::ZERO)
            std::cout << "emmision: " << emmision << "\n";
        std::cout << "--------------------\n";
    }
	
protected:

    Vector3d normal, a, b, c;
    double d, size; // record plane
    BoundPlane planeX, planeY, planeZ; // bound plane
};
