#pragma once

#include "hit.h"

inline Hit::Hit() {
    o = NULL;
    t = 1e38;
    into = true;
}

inline Hit::Hit(double _t, Object3D* _o, const Vector3d &n,
    const Vector2d &_texCoor, const bool &_into) {

    t = _t;
    o = _o;
    geoN = shadeN = n.normalized();
    texCoor = _texCoor;
    into = _into;
}

inline double Hit::getT() const {return t;}

inline Object3D* Hit::getObject() const {return o;}

inline Vector3d Hit::getGeoNormal() const {return geoN;}
inline Vector3d Hit::getShadeNormal() const {return shadeN;}
inline void Hit::setShadeNormal(const Vector3d &_n) {shadeN = _n;}

inline bool Hit::getInto() const {return into;}

inline Vector2d Hit::getTexCoor() const {return texCoor;}

inline Vector3d Hit::getColor() const {return color;}

inline void Hit::setColor(const Vector3d &_color) {color = _color;}

inline void Hit::set(double _t, Object3D* _o, const Vector3d &n,
    const Vector2d &_texCoor, const bool &_into) {

    t = _t;
    o = _o;
    geoN = shadeN = n.normalized();
    texCoor = _texCoor;
    into = _into;
}

inline void Hit::getTangent(Vector3d &_pu, Vector3d &_pv) {_pu = pu; _pv = pv;}
inline void Hit::setTangent(const Vector3d &_pu, const Vector3d &_pv) {
    pu = _pu; pv = _pv;
}