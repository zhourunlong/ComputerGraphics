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
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
        vertices[0] = a; vertices[1] = b; vertices[2] = c;
        normal = Vector3f::cross(b - a, c - a).normalized();
        d = Vector3f::dot(normal, a);
        size = Vector3f::dot(normal, Vector3f::cross(c - b, a - b));
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
        Vector3f a = vertices[0], b = vertices[1], c = vertices[2];

        // Barycentric coordinates
        float alpha = Vector3f::dot(normal, Vector3f::cross(c - b, q - b));
        if (alpha < 0 || alpha > size) return false;

        float beta = Vector3f::dot(normal, Vector3f::cross(q - a, c - a));
        if (beta >= 0 && alpha + beta <= size) {
            h.set(t, material, normal);
            return true;
        }

        return false;
	}
	
protected:

    Vector3f normal, vertices[3];
    float d, size; // record plane
};

#endif //TRIANGLE_H
