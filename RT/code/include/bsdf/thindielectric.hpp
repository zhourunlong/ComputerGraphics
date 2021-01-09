#pragma once

#include "material.hpp"

class ThinDielectric : public Material {
public:
    inline ThinDielectric() {needLS = false;}

    inline void sampleBSDF(const Vector3d &wo, Vector3d &wi,
        Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse)
        override {
        
        lastDiffuse = false;
        Vector3d n = hit.getShadeNormal();
        double eta = intIor / extIor, // eta = n_in / n_out
               cosi = Vector3d::dot(wo, n), cost;
        double F = Fresnel(cosi, cost, eta), r = 2 * F / (1 + F);

        // for simplicity, assume perfect reflection and transmission
        if (r == 1 || sampler->sampleDouble() < r)
            wi = 2 * cosi * n - wo;
        else
            wi = -wo;
        f = Vector3d(1) / cosi;
    }
};
