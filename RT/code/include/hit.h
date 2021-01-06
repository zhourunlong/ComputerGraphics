#pragma once

#include "vecmath/vecmath.h"
#include "ray.hpp"
#include "bsdf/material.hpp"

class Object3D;
class Vector3d;

class Material;

class Hit {
public:
    Hit();

    Hit(double _t, Object3D* _o, Material* _m, const Vector3d &n,
        const Vector2d &_texCoor, const bool &_into);

    ~Hit() = default;

    double getT() const;

    Object3D* getObject() const;

    void setMaterial(Material* _m);
    Material* getMaterial() const;

    Vector3d getGeoNormal() const;
    Vector3d getShadeNormal() const;
    void setShadeNormal(const Vector3d &_n);

    void setEmission(const Vector3d &_emission);
    Vector3d getEmission();

    void setSampleable(bool _sampleable);
    bool isSampleable();

    bool getInto() const;

    Vector2d getTexCoor() const;

    void set(double _t, Object3D* _o, Material* _m, const Vector3d &n,
        const Vector2d &_texCoor, const bool &_into);
    
    Vector3d getColor() const;
    void setColor(const Vector3d &_color);

    void getTangent(Vector3d &_pu, Vector3d &_pv);
    void setTangent(const Vector3d &_pu, const Vector3d &_pv);

protected:
    double t;
    Object3D* o;
    Material* m;
    Vector3d geoN, shadeN, color, pu, pv, emission;
    Vector2d texCoor;
    bool into, sampleable;
};
