#pragma once

#include "material.hpp"

class RoughConductor : public Material {
public:

    inline RoughConductor() {
        needLS = true;
    }
    inline Vector3d getColor(const Vector3d &woo, const Vector3d &wii,
        Hit &hit) override {
    
        Vector3d n = hit.getShadeNormal(), x, y;
        computeBasis(n, x, y);
        Vector3d wo(Vector3d::dot(woo, x),
                    Vector3d::dot(woo, y),
                    Vector3d::dot(woo, n)),
                 wi(Vector3d::dot(wii, x),
                    Vector3d::dot(wii, y),
                    Vector3d::dot(wii, n));
        Vector3d wm = (wo + wi).normalized();
        double cos = Vector3d::dot(wm, wo);
        Vector3d Fres = FresnelConductor(cos, eta, k);
        // Fresnel for dielectric-conductor
        // https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations/
        // extEta not used here
        specRefl->albedo(hit);
        return hit.getColor() * Fres * GGX_D(wm, alpha)
             * Smith_G(wo, wi, wm, alpha) / (4 * wo.z() * wi.z());
    }

    inline void sampleBSDF(const Vector3d &woo, Vector3d &wii,
        Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse)
        override {

        lastDiffuse = true;

        Vector3d n = hit.getShadeNormal(), x, y;
        computeBasis(n, x, y);
        Vector3d wo(Vector3d::dot(woo, x),
                    Vector3d::dot(woo, y),
                    Vector3d::dot(woo, n)),
                 wm, wi;
        wm = GgxVndf(wo, alpha,
                     sampler->sampleDouble(),
                     sampler->sampleDouble());
        wi = 2 * Vector3d::dot(wm, wo) * wm - wo;
        wii = wi.x() * x + wi.y() * y + wi.z() * n;
        double pdf = Smith_G1(wi, wm, alpha)
                   * GGX_D(wm, alpha) * abs(Vector3d::dot(wo, wm))
                   / (4 * abs(Vector3d::dot(wi, wm)) * abs(wo.z()));
        if (pdf == 0) f = Vector3d::ZERO;
        else f = getColor(woo, wii, hit) / pdf;
        // see roughplastic.hpp
    }
};
