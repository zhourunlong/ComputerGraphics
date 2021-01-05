#pragma once

#include "material.hpp"

class Conductor : public Material {
public:
    inline Conductor() {needLS = false;}

    inline void sampleBSDF(const Vector3d &wo, Vector3d &wi,
        Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse)
        override {

        lastDiffuse = false;
        Vector3d n = hit.getShadeNormal();
        double cost = Vector3d::dot(wo, n);
        wi = 2 * cost * n - wo;
        // atomic distribution at wi
        // pdf = 1;
        // color = specRefl; 
        // f = color / pdf / cost;
        specRefl->albedo(hit);
        f = hit.getColor() / abs(cost);
    }
};
