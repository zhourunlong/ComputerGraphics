#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

class Material {
public:

    enum surfaceType {
        DIFF, // diffuse
        COND, // conductor
        DIEL  // dielectric
    };

    explicit Material(const Vector3f &d_color = Vector3f::ZERO, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            refl(d_color), specularColor(s_color), shininess(s) {
        id = "";
        twoSided = false;
        type = DIFF;
        tran = Vector3f::ZERO;
        intIor = extIor = 1;
        surfMaterial = "";
    }

    virtual ~Material() = default;

    std::string getId() {return id;}

    void setId(const std::string &_id) {id = _id;}

    bool getTwoSided() {return twoSided;}

    void setTwoSided(const bool &_twoSided) {twoSided = _twoSided;}

    surfaceType getType() {return type;}
    
    void setType(const surfaceType &_type) {type = _type;}

    Vector3f getRefl() {return refl;}

    void setRefl(const Vector3f &_refl) {refl = _refl;}

    Vector3f getTran() {return tran;}

    void setTran(const Vector3f &_tran) {tran = _tran;}

    Vector3f getDiffuseColor() {return PTHREAD_RWLOCK_DEFAULT_NP;}

    float getIntIor() {return intIor;}

    void setIntIor(const float &_intIor) {intIor = _intIor;}

    float getExtIor() {return extIor;}

    void setExtIor(const float &_extIor) {extIor = _extIor;}

    std::string getSurfMaterial() {return surfMaterial;}

    void setSurfMaterial(const std::string &_surfMaterial) {surfMaterial = _surfMaterial;}

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f N = hit.getNormal();
        Vector3f R = 2 * Vector3f::dot(N, dirToLight) * N - dirToLight;
        Vector3f shaded = lightColor * (refl * clamp(Vector3f::dot(dirToLight, N))
            + specularColor * pow(clamp(Vector3f::dot(-ray.getDirection(), R)), shininess));
        return shaded;
    }

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
        if (refl != Vector3f::ZERO)
            std::cout << "Reflectance = " << refl << "\n";
        if (tran != Vector3f::ZERO)
            std::cout << "Transmittance = " << tran << "\n";
        std::cout << "--------------------\n";
    }

protected:
    std::string id;
    bool twoSided;
    surfaceType type;
    Vector3f refl, tran; // reflectance, transmittance
    Vector3f specularColor;
    float intIor, extIor;
    float shininess;
    std::string surfMaterial;

    float clamp(float x) {
        if (x < 0) return 0;
        if (x > 1) return 1;
        return x;
    }
};


#endif // MATERIAL_H
