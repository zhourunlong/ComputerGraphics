#pragma once

#include "material.hpp"

class Diffuse : public Material {
public:
    inline Diffuse() {needLS = true;}

    inline Vector3d getColor(const Vector3d &wo, const Vector3d &wi,
        const Hit &hit) override {

        return diffRefl->albedo(hit.getTexCoor()) / M_PI;
    }

    inline void sampleBSDF(const Vector3d &wo, Vector3d &wi,
        const Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse)
        override {

        lastDiffuse = true;
        Vector3d z = hit.getNormal(), x, y;
        computeBasis(z, x, y);
        Vector3d t = sampler->sampleHemiSphereCos();
        wi = t.x() * x + t.y() * y + t.z() * z;
        // pdf = t.z() / M_PI;
        // color = diffRefl / M_PI;
        // f = color / pdf;
        f = diffRefl->albedo(hit.getTexCoor()) / t.z();
    }
};
