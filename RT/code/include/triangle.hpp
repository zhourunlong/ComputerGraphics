#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;


class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
    Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c)
        : a(a), b(b), c(c) {
        calc();
	}

	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m)
        : Object3D(m), a(a), b(b), c(c) {
        calc();
	}

    void calc() {
        normal = Vector3f::cross(b - a, c - a);
        size = normal.length();
        normal.normalize();
        d = Vector3f::dot(normal, a);
        
        float Min = a.x(), Max = a.x();
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

	bool intersect(const Ray& r,  Hit& h, float tmin) override {
        Vector3f p = r.getOrigin(), v = r.getDirection();

        // parallel 
        float nv = Vector3f::dot(normal, v);
        if (nv == 0) return false;

        float t = (d - Vector3f::dot(normal, p)) / nv;

        // discard point out of range
        if (t < tmin || t >= h.getT()) return false;

        Vector3f q = r.pointAtParameter(t);

        // Barycentric coordinates
        float alpha = Vector3f::dot(normal, Vector3f::cross(c - b, q - b));
        if (alpha < 0 || alpha > size) return false;

        float beta = Vector3f::dot(normal, Vector3f::cross(q - a, c - a));
        if (beta >= 0 && alpha + beta <= size) {
            if (Vector3f::dot(v, normal) < 0)
                h.set(t, material, normal);
            else 
                h.set(t, material, -normal);
            return true;
        }

        return false;
	}

    BoundPlane getBoundPlaneX() override {return planeX;}
    BoundPlane getBoundPlaneY() override {return planeY;}
    BoundPlane getBoundPlaneZ() override {return planeZ;}

    void print() override {
        std::cout << "===== Triangle =====\n";
        std::cout << a << "\n" << b << "\n" << c << "\n";
        std::cout << "material: " << ref << "\n";
        material->print();
        if (emmision != Vector3f::ZERO)
            std::cout << "emmision: " << emmision << "\n";
        std::cout << "--------------------\n";
    }
	
protected:

    Vector3f normal, a, b, c;
    float d, size; // record plane
    BoundPlane planeX, planeY, planeZ; // bound plane
};

#endif //TRIANGLE_H
