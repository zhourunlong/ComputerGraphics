#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

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
};

class BezierCurve : public Curve {
public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    CurvePoint evaluate(double t) override {
        std::vector <Vector3f> p = controls, d(controls.size(), 0);
        for (int j = p.size(); j > 1; --j)
            for (int i = 0; i < j - 1; ++i) {
                d[i] = (1 - t) * d[i] + t * d[i + 1] + p[i + 1] - p[i];
                p[i] = (1 - t) * p[i] + t * p[i + 1];
            }
        return (CurvePoint){p[0], d[0].normalized()};
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        for (int ti = 0; ti <= resolution; ++ti)
            data.push_back(evaluate(1.0 * ti / resolution));
    }

protected:

};

class BsplineCurve : public Curve {
public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }

    CurvePoint evaluate(double t) override {
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
        Vector3f p(0), d(0);
        for (int i = 0; i <= n; ++i) {
            p += B[i][k] * controls[i];
            d += k * (B[i][k - 1] / (tBuf[i + k] - tBuf[i]) - B[i + 1][k - 1] / (tBuf[i + k + 1] - tBuf[i + 1])) * controls[i];
        }
        return (CurvePoint){p, d.normalized()};
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

protected:

};

#endif // CURVE_HPP
