#pragma once

#include "material.hpp"

class Dielectric : public Material {
public:
    inline Dielectric() {needLS = false;}

    inline void sampleBSDF(const Vector3d &wo, Vector3d &wi,
        Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse)
        override {
        
        lastDiffuse = false;
        Vector3d n = hit.getShadeNormal();
        double eta = hit.getInto() ? intIor / extIor : extIor / intIor, // eta = n_in / n_out
               cosi = Vector3d::dot(wo, n), cost;
        double r = Fresnel(cosi, cost, eta);

        if (r == 1 || sampler->sampleDouble() < r) {
            wi = 2 * cosi * n - wo;
            specRefl->albedo(hit);
            f = hit.getColor() / cosi;
        } else {
            wi = (-wo / eta - (cost - cosi / eta) * n).normalized();
            tran->albedo(hit);
            f = hit.getColor() / (cost * eta * eta);
            // eta^-2: convert energy to radiance
            // http://web.cs.wpi.edu/~emmanuel/courses/cs563/S10/talks/wk11_p2_wadii_simple_transparency.pdf
        }
    }
};
