#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>

class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f N = hit.getNormal();
        Vector3f R = 2 * Vector3f::dot(N, dirToLight) * N - dirToLight;
        Vector3f shaded = lightColor * (diffuseColor * clamp(Vector3f::dot(dirToLight, N))
            + specularColor * pow(clamp(Vector3f::dot(-ray.getDirection(), R)), shininess));
        return shaded;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;

    float clamp(float x) {
        if (x < 0) return 0;
        if (x > 1) return 1;
        return x;
    }
};


#endif // MATERIAL_H
