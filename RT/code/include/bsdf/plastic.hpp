#pragma once

#include "material.hpp"

class Plastic : public Material {
public:
    inline Plastic() {
        needLS = true;
        Fdr = -1;
    }

    inline Vector3d getColor(const Vector3d &wo, const Vector3d &wi,
        Hit &hit) override {
        
        Vector3d n = hit.getShadeNormal();
        double eta = intIor / extIor, cost = Vector3d::dot(wi, n);
        double sint = sqrt(1 - cost * cost) / eta;
        if (sint >= 1) return Vector3d::ZERO;
        cost = sqrt(1 - sint * sint);
        double tmp;
        double T = (1 - Fresnel(Vector3d::dot(wo, n), tmp, eta))
                 * (1 - Fresnel(cost, tmp, eta));
        if (Fdr == -1) computeFdr();
        diffRefl->albedo(hit);
        Vector3d color = hit.getColor() / M_PI;
        return T * color / (1 - color * Fdr) / (eta * eta);
        
        // https://hal.inria.fr/hal-01386157/document
    }

    inline void sampleBSDF(const Vector3d &wo, Vector3d &wi,
       Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse) override {

        Vector3d n = hit.getShadeNormal();
        double cosi = Vector3d::dot(wo, n), cost;
        double r = Fresnel(cosi, cost, intIor / extIor);

        if (r == 1 || sampler->sampleDouble() < r) {
            lastDiffuse = false;
            wi = 2 * cosi * n - wo;
            specRefl->albedo(hit);
            f = hit.getColor() / cosi;
        } else {
            lastDiffuse = true;
            Vector3d x, y;
            computeBasis(n, x, y);
            Vector3d t = sampler->sampleHemiSphereCos();
            wi = t.x() * x + t.y() * y + t.z() * n;
            // pdf = t.z() / M_PI;
            // f = color / pdf; 
            f = M_PI / t.z() * getColor(wo, wi, hit);
        }
    }

protected:
    double Fdr;
    
    void computeFdr() {
        unsigned short Xi[3] = {0, 0, 123};
        Sampler* sampler = new Sampler(Xi);
        double s = 0, tmp;
        int N = 1000000;
        for (int i = 0; i < N; ++i) {
            Vector3d wo = sampler->sampleHemiSphereCos();
            s += Fresnel(wo.z(), tmp, extIor / intIor);
        }
        Fdr = s / N;
    }
};
