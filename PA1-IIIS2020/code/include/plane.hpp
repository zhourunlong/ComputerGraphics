#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class Plane : public Object3D {
public:
    Plane() {
        normal = Vector3f(1, 0, 0);
        d = 0;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal.normalized();
        this->d = d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f p = r.getOrigin(), v = r.getDirection();

        // parallel 
        float nv = Vector3f::dot(normal, v);
        if (nv == 0) return false;

        // otherwise
        float t = (d - Vector3f::dot(normal, p)) / nv;
        if (t >= tmin && t < h.getT()) {
            h.set(t, material, normal);
            return true;
        }
        return false;
    }

protected:

    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

