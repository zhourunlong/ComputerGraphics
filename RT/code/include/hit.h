#pragma once

#include "vecmath/vecmath.h"
#include "ray.hpp"

class Object3D;
class Vector3d;

class Hit {
public:
    Hit();

    Hit(double _t, Object3D* _o, const Vector3d &n, const bool &_into);

    ~Hit() = default;

    double getT() const;

    Object3D* getObject() const;

    Vector3d getNormal() const;

    bool getInto() const;

    void set(double _t, Object3D* _o, const Vector3d &n, const bool &_into);

private:
    double t;
    Object3D* o;
    Vector3d normal;
    bool into;
};