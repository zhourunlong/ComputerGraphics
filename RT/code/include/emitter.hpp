#pragma once

#include <bits/stdc++.h>
#include "vecmath/vecmath.h"
#include "ray.hpp"

class Emitter {
public:
    virtual void setRadiance(const Vector3d &_rad) = 0;
    
    virtual Vector3d radiance(const Ray &r) = 0;
};

class ConstEmitter: public Emitter {
public:
    ConstEmitter() {}

    ConstEmitter(const Vector3d _rad) {rad = _rad;}

    inline void setRadiance(const Vector3d &_rad) override {rad = _rad;}

    inline Vector3d radiance(const Ray &r) override {return rad;}

protected:
    Vector3d rad;
};