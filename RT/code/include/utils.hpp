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

inline Vector3d FresnelConductor(const double &cos,
    const Vector3d &eta, const Vector3d &k) {
    
    Vector3d t = eta * eta + k * k, r = eta * cos;
    Vector3d Rs = Vector3d(1) - 4 * r / (t + 2 * r + Vector3d(cos * cos)),
             Rp = Vector3d(1) - 4 * r / (t * cos * cos + 2 * r + Vector3d(1));
    return 0.5 * (Rs + Rp);
}

inline bool normalFromRefraction(const Vector3d &wo, const Vector3d &wi,
    Vector3d &wm, const double &eta) {
    
    double c = Vector3d::dot(wo, wi);
    double coef = 1/ (eta * eta) + 1 + 2 * c / eta;
    if (coef < 1e-9) return false;
    double y = 1 / sqrt(coef);
    double x = y / eta;
    double t1 = x + c * y;
    if (t1 < 0 || t1 > 1) return false;
    double t2 = c * x + y;
    if (t2 > 0 || t2 < -1) return false;
    wm = x * wo + y * wi;
    return true;
}

inline void computeBasis(const Vector3d &z, Vector3d &x, Vector3d &y) {
    x = Vector3d::cross(fabs(z.x()) > 0.1 ? Vector3d(0, 1, 0)
        : Vector3d(1, 0, 0), z).normalized();
    y = Vector3d::cross(z, x);
}

inline double tanTheta(const double &cost) {
    return sqrt(1 - cost * cost) / cost;
}

inline double tanTheta2(const double &cost) {
    return 1 / (cost * cost) - 1;
}

// https://zhuanlan.zhihu.com/p/20119162
inline double GGX_D(const Vector3d &wm, const double &alpha) {
	double cost2 = wm.z() * wm.z();

	double root = alpha / (1 + (alpha * alpha - 1) * cost2);

	return root * root / M_PI;
}

inline double Smith_G1(const Vector3d &v, Vector3d wm,
    const double &alpha) {

    if (wm.z() < 0) wm = -wm;

    double tant = abs(tanTheta(v.z()));

    if (tant == 0.0) return 1.0;

    if (Vector3d::dot(v, wm) * v.z() <= 0) return 0.0;

    double root = alpha * tant;
    return 2.0 / (1 + sqrt(1 + root * root));
}

inline double Smith_G(const Vector3d &wo, const Vector3d &wi,
    const Vector3d &wm, const double &alpha) {

    return Smith_G1(wo, wm, alpha) * Smith_G1(wi, wm, alpha);
}

// https://schuttejoe.github.io/post/ggximportancesamplingpart2/
Vector3d GgxVndf(Vector3d wo, const double &roughness,
    const double &u1, const double &u2) {
        
    double ttt = 1;
    if (wo.z() < 0) {
        wo = Vector3d(wo.x(), wo.y(), -wo.z());
        ttt = -1;
    }
    Vector3d v = Vector3d(wo.x() * roughness,
                          wo.y() * roughness,
                          wo.z()).normalized(),
             t1, t2;
    computeBasis(v, t1, t2);
    double a = 1.0f / (1.0f + v.z());
    double r = sqrtf(u1);
    double phi = (u2 < a) ? (u2 / a) * M_PI 
                         : M_PI + (u2 - a) / (1.0f - a) * M_PI;
    double p1 = r * cos(phi);
    double p2 = r * sin(phi) * ((u2 < a) ? 1.0f : v.z());
    Vector3d n = p1 * t1 + p2 * t2
               + sqrt(std::max(0.0, 1.0 - p1 * p1 - p2 * p2)) * v;
    return Vector3d(roughness * n.x(),
                    roughness * n.y(),
                    std::max(0.0, n.z()) * ttt).normalized();
}