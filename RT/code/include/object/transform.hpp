#pragma once

#include "vecmath/vecmath.h"
#include "object3d.hpp"
#include <algorithm>

// transforms a 3D point using a matrix, returning a 3D point
inline static Vector3d transformPoint(const Matrix4d &mat, const Vector3d &point) {
    return (mat * Vector4d(point, 1)).xyz();
}

// transform a 3D directino using a matrix, returning a direction
inline static Vector3d transformDirection(const Matrix4d &mat, const Vector3d &dir) {
    return (mat * Vector4d(dir, 0)).xyz();
}

class Transform : public Object3D {
public:
    Transform() {
        objType = TRANSFORM;
    }
    
    inline void appendTransform(const Matrix4d &m) {
        transform = m * transform;
        inv = transform.inverse();
        invTranspose = inv.transposed();
    }

    inline void setObject(Object3D* _o) {o = _o;}

    inline Object3D* getObject() {return o;}

    inline std::string getMatRef() override {return o->getMatRef();}

    inline Material* getMaterial() override {return o->getMaterial();}

    inline void setMaterial(Material* _material) override {o->setMaterial(_material);}

    inline Matrix4d getTransform() {return transform;}

    inline Vector3d getEmmision() override {return o->getEmmision();}

    inline virtual bool intersect(const Ray &r, Hit &h, const double &tmin, const bool &testLs = false) {
        Vector3d trSource = transformPoint(inv, r.getOrigin());
        Vector3d trDirection = transformDirection(inv, r.getDirection());
        Ray tr(trSource, trDirection);
        bool inter = o->intersect(tr, h, tmin, testLs);
        if (inter)
            h.set(h.getT(), h.getObject(),
                transformDirection(invTranspose, h.getNormal()).normalized(),
                h.getTexCoor(), h.getInto());
        return inter;
    }

    inline bool getSample(const Vector3d &x, Vector3d &y, Vector3d &ny, double &A, Sampler* sampler) override {
        bool ret = o->getSample(transformPoint(inv, x), y, ny, A, sampler);
        if (!ret) return false;
        y = transformPoint(transform, y);
        ny = transformDirection(transform, ny).normalized();
        assert(o->getObjType() == TRIANGLE);
        Triangle* t = static_cast<Triangle*>(o);
        Vector3d a = transformPoint(transform, t->getA()),
                 b = transformPoint(transform, t->getB()),
                 c = transformPoint(transform, t->getC());
        A = Vector3d::cross(b - a, c - a).length() / 2;
        return true;
    }

    inline BoundPlane getBoundPlaneX() override {return planeX;}
    inline BoundPlane getBoundPlaneY() override {return planeY;}
    inline BoundPlane getBoundPlaneZ() override {return planeZ;}

    inline void print() override {
        std::cout << "===== Transform =====\n";
        std::cout << "matrix:\n";
        transform.print();
        o->print();
        std::cout << "---------------------\n";
    }

    inline void finish() override {
        o->finish();
        Vector3d p[8];
        BoundPlane oX = o->getBoundPlaneX(),
                   oY = o->getBoundPlaneY(),
                   oZ = o->getBoundPlaneZ();
        p[0] = Vector3d(oX.coorMin, oY.coorMin, oZ.coorMin);
        p[1] = Vector3d(oX.coorMin, oY.coorMin, oZ.coorMax);
        p[2] = Vector3d(oX.coorMin, oY.coorMax, oZ.coorMin);
        p[3] = Vector3d(oX.coorMin, oY.coorMax, oZ.coorMax);
        p[4] = Vector3d(oX.coorMax, oY.coorMin, oZ.coorMin);
        p[5] = Vector3d(oX.coorMax, oY.coorMin, oZ.coorMax);
        p[6] = Vector3d(oX.coorMax, oY.coorMax, oZ.coorMin);
        p[7] = Vector3d(oX.coorMax, oY.coorMax, oZ.coorMax);
        for (int i = 0; i < 8; ++i)
            p[i] = transformPoint(transform, p[i]);
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
    }

protected:
    Object3D *o = NULL; //un-transformed object
    Matrix4d transform = Matrix4d::identity(),
             inv = Matrix4d::identity(),
             invTranspose = Matrix4d::identity();
    BoundPlane planeX, planeY, planeZ;
};
