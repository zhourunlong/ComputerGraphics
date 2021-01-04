#pragma once

#include <bits/stdc++.h>
#include "../vecmath/vecmath.h"

#include "../ray.hpp"
#include "../hit.h"
#include "../hit.cpp"
#include "../sampler.hpp"
#include "../utils.hpp"

class Material {
public:

    enum SurfaceType {
        DIFFUSE,
        CONDUCTOR,
        DIELECTRIC,
        PLASTIC,
        ROUGHPLASTIC
    };

    inline Material() {
        id = "";
        twoSided = false;
        type = DIFFUSE;
    }

    virtual ~Material() = default;

    inline std::string getId() {return id;}
    inline void setId(const std::string &_id) {id = _id;}

    inline bool getTwoSided() {return twoSided;}
    inline void setTwoSided(const bool &_twoSided) {twoSided = _twoSided;}

    inline SurfaceType getType() {return type;}
    inline void setType(const SurfaceType &_type) {type = _type;}

    inline Vector3d getSpecRefl() {return specRefl;}
    inline void setSpecRefl(const Vector3d &_specRefl) {specRefl = _specRefl;}

    inline Vector3d getDiffRefl() {return diffRefl;}
    inline void setDiffRefl(const Vector3d &_diffRefl) {diffRefl = _diffRefl;}

    inline Vector3d getTran() {return tran;}
    inline void setTran(const Vector3d &_tran) {tran = _tran;}

    inline double getIntIor() {return intIor;}
    inline void setIntIor(const double &_intIor) {intIor = _intIor;}

    inline double getExtIor() {return extIor;}
    inline void setExtIor(const double &_extIor) {extIor = _extIor;}

    inline double getAlpha() {return alpha;}
    inline void setAlpha(const double &_alpha) {alpha = _alpha;}

    inline bool needLightSampling() {return needLS;}

    inline virtual Vector3d getColor(const Vector3d &wo,
        const Vector3d &wi, const Hit &hit) {return Vector3d::ZERO;}

    inline virtual void sampleBSDF(const Vector3d &wo, Vector3d &wi,
        const Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse) {}

    inline void print() {
        std::cout << "===== Material =====\n";
        std::cout << "id: " << id << "\n";
        if (twoSided)
            std::cout << "two sided\n";
        switch (type) {
            case DIFFUSE:
                std::cout << "diffuse\n";
                break;
            case CONDUCTOR:
                std::cout << "conductor\n";
                break;
            case DIELECTRIC:
                std::cout << "dielectric\n";
                std::cout << "int ior = " << intIor << "\n";
                std::cout << "ext ior = " << extIor << "\n";
                break;
            case PLASTIC:
                std::cout << "plastic\n";
                std::cout << "int ior = " << intIor << "\n";
                std::cout << "ext ior = " << extIor << "\n";
                break;
            case ROUGHPLASTIC:
                std::cout << "rough plastic\n";
                std::cout << "int ior = " << intIor << "\n";
                std::cout << "ext ior = " << extIor << "\n";
                std::cout << "alpha = " << alpha << "\n";
                break;
        }
        if (specRefl != Vector3d::ZERO)
            std::cout << "Specular Reflectance = " << specRefl << "\n";
        if (diffRefl != Vector3d::ZERO)
            std::cout << "Diffuse Reflectance = " << diffRefl << "\n";
        if (tran != Vector3d::ZERO)
            std::cout << "Transmittance = " << tran << "\n";
        std::cout << "--------------------\n";
    }

protected:
    std::string id;
    bool twoSided, needLS;
    SurfaceType type;
    Vector3d specRefl, diffRefl, tran; // specular & diffuse reflectance, transmittance
    double intIor, extIor, alpha;
};