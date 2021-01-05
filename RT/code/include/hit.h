#pragma once

#include "vecmath/vecmath.h"
#include "ray.hpp"

class Object3D;
class Vector3d;

class Hit {
public:
    Hit();

    Hit(double _t, Object3D* _o, const Vector3d &n, const Vector2d &_texCoor,
        const bool &_into);

    ~Hit() = default;

    double getT() const;

    Object3D* getObject() const;

    Vector3d getGeoNormal() const;
    Vector3d getShadeNormal() const;
    void setShadeNormal(const Vector3d &_n);

    bool getInto() const;

    Vector2d getTexCoor() const;

    void set(double _t, Object3D* _o, const Vector3d &n,
        const Vector2d &_texCoor, const bool &_into);
    
    Vector3d getColor() const;
    void setColor(const Vector3d &_color);

    void getTangent(Vector3d &_pu, Vector3d &_pv);
    void setTangent(const Vector3d &_pu, const Vector3d &_pv);

private:
    double t;
    Object3D* o;
    Vector3d geoN, shadeN, color, pu, pv;
    Vector2d texCoor;
    bool into;
};
