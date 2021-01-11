#pragma once

#include "material.hpp"

class RoughPlastic : public Material {
public:
    inline RoughPlastic() {
        needLS = true;
        Fdr = -1;
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
        double eta = intIor / extIor;
        double sint = sqrt(1 - wi.z() * wi.z()) / eta;
        if (sint >= 1) return Vector3d::ZERO;
        double cost = sqrt(1 - sint * sint), tmp;
        double T = (1 - Fresnel(abs(wo.z()), tmp, eta))
                 * (1 - Fresnel(cost, tmp, eta));
        if (Fdr == -1) computeFdr();
        diffRefl->albedo(hit);
        Vector3d color = hit.getColor() / M_PI;
        Vector3d diff = T * color / (1 - color * Fdr) / (eta * eta);
        Vector3d wm = (wo + wi).normalized();
        specRefl->albedo(hit);
        Vector3d spec = hit.getColor() * GGX_D(wm, alpha)
                      * Fresnel(Vector3d::dot(wo, wm), tmp, eta)
                      * Smith_G(wo, wi, wm, alpha)
                      / (4 * wo.z() * wi.z());
        return diff + spec;
        // https://hal.inria.fr/hal-01386157/document
        // https://zhuanlan.zhihu.com/p/20119162
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
        if (sampler->sampleDouble() < 0.5) {
            do {
                wm = GgxVndf(wo, alpha,
                             sampler->sampleDouble(),
                             sampler->sampleDouble());
            } while (abs(Vector3d::dot(wm, wo)) < 1e-9);
            wi = 2 * Vector3d::dot(wm, wo) * wm - wo;
            // pdf1
        } else {
            wi = sampler->sampleHemiSphereCos();
            wm = (wo + wi).normalized(); 
            // pdf2 = wi.z() / M_PI;
        }
        wii = wi.x() * x + wi.y() * y + wi.z() * n;
        double pdf1 = Smith_G1(wi, wm, alpha)
                    * GGX_D(wm, alpha) * abs(Vector3d::dot(wo, wm))
                    / (4 * abs(Vector3d::dot(wi, wm)) * abs(wo.z())),
               pdf2 = abs(wi.z()) / M_PI;
		double pdf = (pdf1 + pdf2) / 2;
        f = getColor(woo, wii, hit) / pdf;
        // http://jcgt.org/published/0007/04/01/paper.pdf
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
