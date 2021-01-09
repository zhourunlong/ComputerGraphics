#pragma once

#include <bits/stdc++.h>
#include "../vecmath/vecmath.h"
#include "../ray.hpp"
#include "../hit.h"
#include "../hit.cpp"
#include "../sampler.hpp"
#include "../utils.hpp"
#include "../texture/texture.h"
#include "../texture/texture.cpp"
#include "../texture/bump.hpp"

class Material {
public:

    enum SurfaceType {
        DIFFUSE,
        CONDUCTOR,
        ROUGHCONDUCTOR,
        DIELECTRIC,
        THINDIELECTRIC,
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

    inline Texture* getSpecRefl() {return specRefl;}
    inline void setSpecRefl(Texture* _specRefl) {specRefl = _specRefl;}

    inline Texture* getDiffRefl() {return diffRefl;}
    inline void setDiffRefl(Texture* _diffRefl) {diffRefl = _diffRefl;}

    inline Texture* getTran() {return tran;}
    inline void setTran(Texture* _tran) {tran = _tran;}

    inline double getIntIor() {return intIor;}
    inline void setIntIor(const double &_intIor) {intIor = _intIor;}

    inline double getExtIor() {return extIor;}
    inline void setExtIor(const double &_extIor) {extIor = _extIor;}

    inline double getAlpha() {return alpha;}
    inline void setAlpha(const double &_alpha) {alpha = _alpha;}

    inline bool needLightSampling() {return needLS;}

    inline void setEta(const Vector3d &_eta) {eta = _eta;}

    inline void setK(const Vector3d &_k) {k = _k;}

    inline void setExtEta(const double &_extEta) {extEta = _extEta;}

    inline virtual Vector3d getColor(const Vector3d &wo,
        const Vector3d &wi, Hit &hit) {return Vector3d::ZERO;}

    inline virtual void sampleBSDF(const Vector3d &wo, Vector3d &wi,
        Hit &hit, Vector3d &f, Sampler* sampler, bool &lastDiffuse) {}

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
        if (specRefl) {
            std::cout << "Specular Reflectance:\n";
            specRefl->print();
        }
        if (diffRefl) {
            std::cout << "Diffuse Reflectance:\n";
            diffRefl->print();
        }
        if (tran) {
            std::cout << "Transmittance:\n";
            tran->print();
        }
        std::cout << "--------------------\n";
    }

protected:
    std::string id;
    bool twoSided, needLS;
    SurfaceType type;
    Texture* specRefl = NULL;
    Texture* diffRefl = NULL;
    Texture* tran = NULL;
    double intIor, extIor, alpha, extEta;
    Vector3d eta, k;
};
