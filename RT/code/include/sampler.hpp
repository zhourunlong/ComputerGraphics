#pragma once

#include <bits/stdc++.h>
#include "vecmath/vecmath.h"

class Sampler {
public:
    Sampler(unsigned short *_Xi) {Xi = _Xi;}

    inline double sampleDouble() {return erand48(Xi);}

    inline Vector2d sampleDiskUniform() {
        double r = sqrt(erand48(Xi)), theta = 2 * M_PI * erand48(Xi);
        return Vector2d(r * cos(theta), r * sin(theta));
    }

    inline Vector3d sampleThetaSphereUniform(const double &cosThMx) {
        double theta = acos(cosThMx + erand48(Xi) * (1 - cosThMx)),
               phi = 2 * M_PI * erand48(Xi);
        return Vector3d(sin(theta) * cos(phi),
                        sin(theta) * sin(phi),
                        cos(theta));
    }

    inline Vector3d sampleSphereUniform() {
        return sampleThetaSphereUniform(-1);
    }

    inline Vector3d sampleHemiSphereCos() {
        double r1 = 2 * M_PI * erand48(Xi),
               r2 = erand48(Xi);
        double r2s = sqrt(r2);
        return Vector3d(cos(r1) * r2s, 
                        sin(r1) * r2s,
                        sqrt(1 - r2));
    }

    inline Vector2d sampleTriangle() {
        double u = sqrt(erand48(Xi));
        return Vector2d(1 - u, erand48(Xi) * u);
    }

private:
    unsigned short *Xi;
};