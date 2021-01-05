#pragma once

#include "object3d.hpp"
#include <cmath>
#include <iostream>
using namespace std;

class Triangle: public Object3D {

public:
    Triangle() = delete;

    // a b c are three vertex positions of the triangle
    inline Triangle(const Vector3d& a, const Vector3d& b, const Vector3d& c)
        : a(a), b(b), c(c) {
        objType = TRIANGLE;
        calc();
    }

    inline Triangle(const Vector3d& a, const Vector3d& b, const Vector3d& c, Material* m)
        : Object3D(m), a(a), b(b), c(c) {
        objType = TRIANGLE;
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

    inline Vector3d getA() {return a;}
    inline Vector3d getB() {return b;}
    inline Vector3d getC() {return c;}

    inline void setVertexNormal(Vector3d _va, Vector3d _vb, Vector3d _vc) {
        va = _va; vb = _vb; vc = _vc;
        vertexNormal = true;
    }

    inline void setTextureCoordinate(Vector2d _ta, Vector2d _tb, Vector2d _tc) {
        ta = _ta; tb = _tb; tc = _tc;
        textureCoordinate = true;
    }

    inline void disableVertexNormal() {vertexNormal = false;}

    inline bool intersect(const Ray& r,  Hit& h, const double &tmin, const bool &testLs = false) override {
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
            if (testLs)
                if (t < h.getT() - 2e-9) return true;
                else return false;
            Vector2d texCoor = (alpha * ta + beta * tb + (size - alpha - beta) * tc) / size;
            if (Vector3d::dot(v, normal) < 0) {
                h.set(t, this, normal, texCoor, true);
                if (vertexNormal)
                    h.setShadeNormal(((alpha * va + beta * vb + (size - alpha - beta) * vc) / size).normalized());
                h.setTangent(pu, pv);
            } else {
                h.set(t, this, -normal, texCoor, false);
                if (vertexNormal)
                    h.setShadeNormal(-((alpha * va + beta * vb + (size - alpha - beta) * vc) / size).normalized());
                h.setTangent(-pu, -pv);
            }
            return true;
        }

        return false;
    }

    inline void calcTangent() {
        Matrix2d m((tb - ta).x(), (tc - ta).x(),
                   (tb - ta).y(), (tc - ta).y());
        m = m.inverse().transposed();
        Vector2d px((b - a).x(), (c - a).x()),
                 py((b - a).y(), (c - a).y()),
                 pz((b - a).z(), (c - a).z());
        px = m * px; py = m * py; pz = m * pz;
        pu = Vector3d(px.x(), py.x(), pz.x()).normalized();
        pv = Vector3d(px.y(), py.y(), pz.y()).normalized();
    }

    inline bool getSample(const Vector3d &x, Vector3d &y, Vector3d &ny, double &A, Sampler* sampler) override {
        if (Vector3d::dot(normal, x - a) <= 0) return false;
        A = size / 2;
        Vector2d s = sampler->sampleTriangle();
        y = s.x() * a + s.y() * b + (1 - s.x() - s.y()) * c;
        ny = normal;
        return true;
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
    bool vertexNormal = false, textureCoordinate = false;
    Vector3d normal, a, b, c, va, vb, vc;
    Vector2d ta, tb, tc;
    double d, size; // record plane
    BoundPlane planeX, planeY, planeZ; // bound plane
};
