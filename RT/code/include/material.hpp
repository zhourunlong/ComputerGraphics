#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

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

    Material() {
        id = "";
        twoSided = false;
        type = DIFF;
        refl = tran = Vector3d::ZERO;
        intIor = extIor = 1;
        surfMaterial = "";
    }

    virtual ~Material() = default;

    std::string getId() {return id;}

    void setId(const std::string &_id) {id = _id;}

    bool getTwoSided() {return twoSided;}

    void setTwoSided(const bool &_twoSided) {twoSided = _twoSided;}

    SurfaceType getType() {return type;}
    
    void setType(const SurfaceType &_type) {type = _type;}

    Vector3d getRefl() {return refl;}

    void setRefl(const Vector3d &_refl) {refl = _refl;}

    Vector3d getTran() {return tran;}

    void setTran(const Vector3d &_tran) {tran = _tran;}

    double getIntIor() {return intIor;}

    void setIntIor(const double &_intIor) {intIor = _intIor;}

    double getExtIor() {return extIor;}

    void setExtIor(const double &_extIor) {extIor = _extIor;}

    std::string getSurfMaterial() {return surfMaterial;}

    void setSurfMaterial(const std::string &_surfMaterial) {surfMaterial = _surfMaterial;}

    void print() {
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
