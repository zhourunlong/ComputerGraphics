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
        isPlane = true;
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal.normalized();
        this->d = d;
        isPlane = true;
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

    void drawGL() override {
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(normal, xAxis);
        xAxis = Vector3f::cross(yAxis, normal);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(d * normal + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * normal - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * normal + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(normal);
        glVertex3fv(d * normal + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * normal - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * normal - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

    BoundPlane getBoundPlaneX() override {}
    BoundPlane getBoundPlaneY() override {}
    BoundPlane getBoundPlaneZ() override {}

protected:
    Vector3f normal;
    float d;

};

#endif //PLANE_H