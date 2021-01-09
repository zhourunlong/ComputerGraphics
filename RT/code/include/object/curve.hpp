#pragma once

#include <bits/stdc++.h>
#include "object3d.hpp"
#include "../vecmath/vecmath.h"

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    double s;   // parameter
    Vector3d V; // Vertex
    Vector3d T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3d> controls;
public:
    explicit Curve(std::vector<Vector3d> points)
        : controls(std::move(points)) {
        
        objType = CURVE;
    }

    inline bool intersect(const Ray &r, Hit &h, const double &tmin, const bool &testLs = false) override {
        return false;
    }

    std::vector<Vector3d> &getControls() {
        return controls;
    }

    inline int getControlSize() {return controls.size();}

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    virtual CurvePoint evaluate(double t) = 0;

    BoundPlane getBoundPlaneX() override {
        return (BoundPlane){0, 0};
    }

    BoundPlane getBoundPlaneY() override {
        return (BoundPlane){0, 0};
    }

    BoundPlane getBoundPlaneZ() override {
        return (BoundPlane){0, 0};
    }

    inline int numObjects() override {return 1;}

    inline virtual void print() = 0;
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3d> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    CurvePoint evaluate(double _t) override {
        double t = clamp(_t);
        std::vector <Vector3d> p = controls, d(controls.size(), 0);
        for (int j = p.size(); j > 1; --j)
            for (int i = 0; i < j - 1; ++i) {
                d[i] = (1 - t) * d[i] + t * d[i + 1] + p[i + 1] - p[i];
                p[i] = (1 - t) * p[i] + t * p[i + 1];
            }
        Vector3d deri = d[0].normalized();
        if (t == _t) return (CurvePoint){t, p[0], deri};
        if (_t < 0) return (CurvePoint){_t, p[0] + _t * deri, deri};
        return (CurvePoint){_t, p[0] + (_t - 1) * deri, deri};
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        for (int ti = 0; ti <= resolution; ++ti)
            data.push_back(evaluate(1.0 * ti / resolution));
    }
    
    inline void print() override {
        std::cout << "===== Bezier Curve =====\n";
        std::cout << controls.size() << " control points: \n";
        for (int i = 0; i < controls.size(); ++i)
            std::cout << "  " << controls[i] << "\n";
        std::cout << "------------------------\n";
    }
};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3d> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }

    CurvePoint evaluate(double _t) override {
        double t = clamp(_t);
        int n = controls.size() - 1, k = 3;
        double B[n + k + 1][k + 1], tBuf[n + k + 2];
        for (int ti = 0; ti < n + k + 2; ++ti)
            tBuf[ti] = 1.0 * ti / (n + k + 1);
        for (int i = 0; i < n + k + 1; ++i)
            B[i][0] = (tBuf[i] <= t && t < tBuf[i + 1]);
        for (int p = 1; p <= k; ++p)
            for (int i = 0; i < n + k + 1 - p; ++i)
                B[i][p] = B[i][p - 1] * (t - tBuf[i]) / (tBuf[i + p] - tBuf[i])
                        + (tBuf[i + p + 1] - t) / (tBuf[i + p + 1] - tBuf[i + 1]) * B[i + 1][p - 1];
        Vector3d p(0), d(0);
        for (int i = 0; i <= n; ++i) {
            p += B[i][k] * controls[i];
            d += k * (B[i][k - 1] / (tBuf[i + k] - tBuf[i]) - B[i + 1][k - 1] / (tBuf[i + k + 1] - tBuf[i + 1])) * controls[i];
        }
        Vector3d deri = d.normalized();
        if (t == _t) return (CurvePoint){t, p, deri};
        if (_t < 0) return (CurvePoint){_t, p + _t * deri, deri};
        return (CurvePoint){_t, p + (_t - 1) * deri, deri};
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        int n = controls.size() - 1, k = 3;
        int tiMax = (n + 1) * resolution;
        for (int ti = k * resolution; ti <= tiMax; ++ti) {
            double t = 1.0 * ti / (n + k + 1) / resolution;
            data.push_back(evaluate(t));
        }
    }

    inline void print() override {
        std::cout << "===== B-spline Curve =====\n";
        std::cout << controls.size() << " control points: \n";
        for (int i = 0; i < controls.size(); ++i)
            std::cout << "  " << controls[i] << "\n";
        std::cout << "--------------------------\n";
    }
};
