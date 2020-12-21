#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include "vecmath/vecmath.h"

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

class Material {
public:

    enum SurfaceType {
        DIFF, // diffuse
        COND, // conductor
        DIEL  // dielectric
    };

    inline Material() {
        id = "";
        twoSided = false;
        type = DIFF;
        refl = tran = Vector3d::ZERO;
        intIor = extIor = 1;
        surfMaterial = "";
    }

    virtual ~Material() = default;

    inline std::string getId() {return id;}

    inline void setId(const std::string &_id) {id = _id;}

    inline bool getTwoSided() {return twoSided;}

    inline void setTwoSided(const bool &_twoSided) {twoSided = _twoSided;}

    inline SurfaceType getType() {return type;}
    
    inline void setType(const SurfaceType &_type) {type = _type;}

    inline Vector3d getRefl() {return refl;}

    inline void setRefl(const Vector3d &_refl) {refl = _refl;}

    inline Vector3d getTran() {return tran;}

    inline void setTran(const Vector3d &_tran) {tran = _tran;}

    inline double getIntIor() {return intIor;}

    inline void setIntIor(const double &_intIor) {intIor = _intIor;}

    inline double getExtIor() {return extIor;}

    inline void setExtIor(const double &_extIor) {extIor = _extIor;}

    inline std::string getSurfMaterial() {return surfMaterial;}

    inline void setSurfMaterial(const std::string &_surfMaterial) {surfMaterial = _surfMaterial;}

    inline void print() {
        std::cout << "===== Material =====\n";
        std::cout << "id: " << id << "\n";
        if (twoSided)
            std::cout << "two sided\n";
        switch (type) {
            case DIFF:
                std::cout << "diffuse\n";
                break;
            case COND:
                std::cout << "conductor\n";
                std::cout << "surface material = " << surfMaterial << "\n";
                break;
            case DIEL:
                std::cout << "dielectric\n";
                std::cout << "int ior = " << intIor << "\n";
                std::cout << "ext ior = " << extIor << "\n";
                break;
        }
        if (refl != Vector3d::ZERO)
            std::cout << "Reflectance = " << refl << "\n";
        if (tran != Vector3d::ZERO)
            std::cout << "Transmittance = " << tran << "\n";
        std::cout << "--------------------\n";
    }

protected:
    std::string id;
    bool twoSided;
    SurfaceType type;
    Vector3d refl, tran; // reflectance, transmittance
    double intIor, extIor;
    std::string surfMaterial;
};


#endif // MATERIAL_H
