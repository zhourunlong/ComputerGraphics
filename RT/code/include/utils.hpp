#pragma once

#include <bits/stdc++.h>
#include "vecmath/vecmath.h"

inline double clamp(const double &x) {return x < 0 ? 0 : (x > 1 ? 1 : x);}

inline double gammaCorrection(const double &x, const double &filmGamma) {
    return pow(clamp(x), 1.0 / filmGamma);
}

inline Vector3d gammaCorrection(const Vector3d &v, const double &filmGamma) {
    return Vector3d(gammaCorrection(v.x(), filmGamma),
                    gammaCorrection(v.y(), filmGamma),
                    gammaCorrection(v.z(), filmGamma));
}

inline double Fresnel(const double &cosi, double &cost, const double &ior) {
    double sini = sqrt(std::max(0.0, 1 - cosi * cosi));
    double sint = sini / ior;
    if (sint >= 1) return 1;
    cost = sqrt(1 - sint * sint);
    double r1 = (ior * cosi - cost) / (ior * cosi + cost);
    double r2 = (cosi - ior * cost) / (cosi + ior * cost);
    return 0.5 * (r2 * r2 + r1 * r1);
}

inline void computeBasis(const Vector3d &z, Vector3d &x, Vector3d &y) {
    x = Vector3d::cross(fabs(z.x()) > 0.1 ? Vector3d(0, 1, 0)
        : Vector3d(1, 0, 0), z).normalized();
    y = Vector3d::cross(z, x);
}