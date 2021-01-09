#pragma once

#include "hit.h"

inline Hit::Hit() {
    o = NULL;
    t = 1e38;
    into = true;
}

inline Hit::Hit(double _t, Object3D* _o, Material* _m, const Vector3d &n,
    const Vector2d &_texCoor, const bool &_into) {

    t = _t;
    o = _o;
    m = _m;
    geoN = shadeN = n.normalized();
    texCoor = _texCoor;
    into = _into;
}

inline void Hit::setT(const double &_t) {t = _t;}

inline double Hit::getT() const {return t;}

inline void Hit::setObject(Object3D* _o) {o = _o;}
inline Object3D* Hit::getObject() const {return o;}

inline void Hit::setMaterial(Material* _m) {m = _m;}
inline Material* Hit::getMaterial() const {return m;}

inline Vector3d Hit::getGeoNormal() const {return geoN;}
inline Vector3d Hit::getShadeNormal() const {return shadeN;}
inline void Hit::setShadeNormal(const Vector3d &_n) {shadeN = _n;}

inline void Hit::setEmission(const Vector3d &_emission) {
    emission = _emission;
}

inline Vector3d Hit::getEmission() {return emission;}

inline void Hit::setSampleable(bool _sampleable) {sampleable = _sampleable;}

inline bool Hit::isSampleable() {return sampleable;}

inline bool Hit::getInto() const {return into;}

inline Vector2d Hit::getTexCoor() const {return texCoor;}

inline Vector3d Hit::getColor() const {return color;}

inline void Hit::setColor(const Vector3d &_color) {color = _color;}

inline void Hit::set(double _t, Object3D* _o, Material* _m, const Vector3d &n,
    const Vector2d &_texCoor, const bool &_into) {

    t = _t;
    o = _o;
    m = _m;
    geoN = shadeN = n.normalized();
    texCoor = _texCoor;
    into = _into;
}

inline void Hit::getTangent(Vector3d &_pu, Vector3d &_pv) {_pu = pu; _pv = pv;}
inline void Hit::setTangent(const Vector3d &_pu, const Vector3d &_pv) {
    pu = _pu; pv = _pv;
}