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
    normal = n.normalized();
    texCoor = _texCoor;
    into = _into;
}

inline double Hit::getT() const {return t;}

inline Object3D* Hit::getObject() const {return o;}

inline Vector3d Hit::getNormal() const {return normal;}

inline bool Hit::getInto() const {return into;}

inline Vector2d Hit::getTexCoor() const {return texCoor;}

inline void Hit::set(double _t, Object3D* _o, const Vector3d &n,
    const Vector2d &_texCoor, const bool &_into) {

    t = _t;
    o = _o;
    normal = n.normalized();
    texCoor = _texCoor;
    into = _into;
}