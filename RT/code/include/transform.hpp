#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "object3d.hpp"
#include <algorithm>

// transforms a 3D point using a matrix, returning a 3D point
static Vector3f transformPoint(const Matrix4f &mat, const Vector3f &point) {
    return (mat * Vector4f(point, 1)).xyz();
}

// transform a 3D directino using a matrix, returning a direction
static Vector3f transformDirection(const Matrix4f &mat, const Vector3f &dir) {
    return (mat * Vector4f(dir, 0)).xyz();
}

class Transform : public Object3D {
public:
    Transform() {}

    Transform(const Matrix4f &m, Object3D *obj) : o(obj) {
        Vector3f p[8];
        BoundPlane oX = o->getBoundPlaneX(),
                   oY = o->getBoundPlaneY(),
                   oZ = o->getBoundPlaneZ();
        p[0] = Vector3f(oX.coorMin, oY.coorMin, oZ.coorMin);
        p[1] = Vector3f(oX.coorMin, oY.coorMin, oZ.coorMax);
        p[2] = Vector3f(oX.coorMin, oY.coorMax, oZ.coorMin);
        p[3] = Vector3f(oX.coorMin, oY.coorMax, oZ.coorMax);
        p[4] = Vector3f(oX.coorMax, oY.coorMin, oZ.coorMin);
        p[5] = Vector3f(oX.coorMax, oY.coorMin, oZ.coorMax);
        p[6] = Vector3f(oX.coorMax, oY.coorMax, oZ.coorMin);
        p[7] = Vector3f(oX.coorMax, oY.coorMax, oZ.coorMax);
        for (int i = 0; i < 8; ++i)
            p[i] = transformPoint(m, p[i]);
        planeX = (BoundPlane){p[0].x(), p[0].x()};
        planeY = (BoundPlane){p[0].y(), p[0].y()};
        planeZ = (BoundPlane){p[0].z(), p[0].z()};
        for (int i = 1; i < 8; ++i) {
            planeX.coorMin = std::min(planeX.coorMin, p[i].x());
            planeX.coorMax = std::max(planeX.coorMax, p[i].x());
            planeY.coorMin = std::min(planeY.coorMin, p[i].y());
            planeY.coorMax = std::max(planeY.coorMax, p[i].y());
            planeZ.coorMin = std::min(planeZ.coorMin, p[i].z());
            planeZ.coorMax = std::max(planeZ.coorMax, p[i].z());
        }
        transform = m.inverse();
    }
    
    void appendTransform(const Matrix4f &m) {transform = transform * m;}

    void setObject(Object3D* _o) {o = _o;}

    std::string getMatRef() override {return o->getMatRef();}

    Material* getMaterial() override {return o->getMaterial();}

    void setMaterial(Material* _material) override {o->setMaterial(_material);}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) {
        Vector3f trSource = transformPoint(transform, r.getOrigin());
        Vector3f trDirection = transformDirection(transform, r.getDirection());
        Ray tr(trSource, trDirection);
        bool inter = o->intersect(tr, h, tmin);
        if (inter) {
            h.set(h.getT(), h.getObject(), transformDirection(transform.transposed(), h.getNormal()).normalized(), h.getInto());
        }
        return inter;
    }

    BoundPlane getBoundPlaneX() override {return planeX;}
    BoundPlane getBoundPlaneY() override {return planeY;}
    BoundPlane getBoundPlaneZ() override {return planeZ;}

    void print() override {
        std::cout << "===== Transform =====\n";
        std::cout << "matrix:\n";
        transform.print();
        o->print();
        std::cout << "---------------------\n";
    }

protected:
    Object3D *o = NULL; //un-transformed object
    Matrix4f transform = Matrix4f::identity();
    BoundPlane planeX, planeY, planeZ;
};

#endif //TRANSFORM_H
