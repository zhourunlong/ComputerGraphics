#pragma once

#include <bits/stdc++.h>
#include "object3d.hpp"
#include "triangle.hpp"
#include "mesh.hpp"
#include "curve.hpp"

class RevSurface : public Object3D {

    Curve *pCurve;

public:
    RevSurface() {
        objType = REVSURFACE;
        sampleable = false;
    }

    void setCurve(Curve* _pCurve) {
        pCurve = _pCurve;
        double mxx = 0;
        planeY = (BoundPlane){1e38, -1e38};
        for (const auto &cp : pCurve->getControls()) {
            if (cp.z() != 0.0) {
                printf("Profile of revSurface must be flat on xy plane.\n");
                exit(0);
            }

            mxx = max(mxx, abs(cp.x()));
            planeY.coorMin = min(planeY.coorMin, cp.y());
            planeY.coorMax = max(planeY.coorMax, cp.y());
        }
        planeX = planeZ = (BoundPlane){-mxx, mxx};

        buildMesh();
    }

    bool intersect(const Ray &r, Hit &h, const double &tmin, const bool &testLs = false) override {
        /*
        F(t,s,theta)=(p_x+t*v_x-f(s)_x*cos(theta),
                      p_y+t*v_y-f(s)_y,
                      p_z+t*v_z-f(s)_x*sin(theta))
        J_F=[v_x, -cos(theta)*[f(s)_x]', sin(theta)*f(s)_x,
             v_y, -[f(s)_y]',            0,
             v_z, -sin(theta)*[f(s)_x]', -cos(theta)*f(s)_x]
        */
        Hit th = Hit();
        th.setT(h.getT()); 
        bool res = mesh->intersect(r, th, tmin);
        if (!res) return false;

        double t = th.getT(), s, theta = 0;
        Vector3d p = r.pointAtParameter(t);
        double s_xz = sqrt(p.x() * p.x() + p.z() * p.z()), mi = 1e9;
        CurvePoint TP;
        for (auto P: curvePoints) {
            double err = abs(s_xz - abs(P.V.x())) + abs(p.y() - P.V.y());
            if (err < mi) {
                mi = err;
                s = P.s;
                TP = P;
            }
        }
        if (s_xz > 1e-9) {
            theta = acos(p.x() / s_xz);
            if (p.z() * sin(theta) < 0) theta = -theta;
            if (TP.V.x() < 0) theta += M_PI;
        }

        double initial = t;
        double eta = 0.1;
        for (int i = 0; i < NewtonSteps; ++i) {
            CurvePoint fs = pCurve->evaluate(s);
            Vector3d F(p.x() - fs.V.x() * cos(theta),
                       p.y() - fs.V.y(),
                       p.z() - fs.V.x() * sin(theta));
            if (F.length() < eps) {
                if (t >= tmin && t < h.getT()) {
                    if (testLs) return true;
                    Vector3d n(-fs.T.y() * cos(theta), fs.T.x(), -fs.T.y() * sin(theta));
                    n.normalize();
                    if (Vector3d::dot(r.getDirection(), n) < 0)
                        h.set(t, this, material, n, Vector2d::ZERO, true);
                    else
                        h.set(t, this, material, -n, Vector2d::ZERO, false);
                    return true;
                }
                return false;
            }
            
            Matrix3d JF(r.getDirection().x(), -cos(theta) * fs.T.x(), sin(theta) * fs.V.x(),
                        r.getDirection().y(), -fs.T.y(),              0,
                        r.getDirection().z(), -sin(theta) * fs.T.x(), -cos(theta) * fs.V.x());
            Vector3d delta = JF.inverse() * F;
            t -= eta * delta.x(); s -= eta * delta.y(); theta -= eta * delta.z();
            eta *= 0.99;
            if (isnan(t) || isnan(s) || isnan(theta))
                return false;
        }
        CurvePoint fs = pCurve->evaluate(s);
        Vector3d F(p.x() - fs.V.x() * cos(theta),
                       p.y() - fs.V.y(),
                       p.z() - fs.V.x() * sin(theta));
        return false;
    }

    inline BoundPlane getBoundPlaneX() override {return planeX;}

    inline BoundPlane getBoundPlaneY() override {return planeY;}

    inline BoundPlane getBoundPlaneZ() override {return planeZ;}

    inline int numObjects() override {return 1;}

    inline void print() override {
        std::cout << "===== Curve Revolving Body =====\n";
        std::cout << "material: " << ref << "\n";
        if (material) material->print();
        pCurve->print();
        std::cout << "--------------------------------\n";
    }

protected:
    void buildMesh() {
        struct TriangleIndex {int i, j, k;};
        std::vector <TriangleIndex> VF;
        std::vector <Vector3d> VV;
        std::vector <Object3D*> triangles;
        pCurve->discretize(resolution, curvePoints);
        for (unsigned int ci = 0; ci < curvePoints.size(); ++ci) {
            const CurvePoint &cp = curvePoints[ci];
            for (unsigned int i = 0; i < steps; ++i) {
                double t = 1.0 * i / steps;
                Quat4d rot;
                rot.setAxisAngle(t * 2 * M_PI, Vector3d::UP);
                Vector3d pnew = Matrix3d::rotation(rot) * cp.V;
                VV.push_back(pnew);
                int i1 = (i + 1 == steps) ? 0 : i + 1;
                if (ci != curvePoints.size() - 1) {
                    VF.push_back((TriangleIndex){(ci + 1) * steps + i, ci * steps + i1, ci * steps + i});
                    VF.push_back((TriangleIndex){(ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1});
                }
            }
        }
        for (int i = 0; i < VF.size(); ++i) {
            Triangle* tri = new Triangle(VV[VF[i].i], VV[VF[i].j], VV[VF[i].k], NULL);
            triangles.push_back(tri);
        }
        mesh = new Mesh();
        mesh->setTriangles(triangles);
    }

    const double eps = 1e-2;
    const int resolution = 100, steps = 100, NewtonSteps = 100;
    BoundPlane planeX, planeY, planeZ;
    Mesh* mesh;
    std::vector <CurvePoint> curvePoints;
};
