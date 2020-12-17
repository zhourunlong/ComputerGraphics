#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "ray.hpp"
#include "object3d.hpp"

class Material;

class Hit {
public:

    // constructors
    Hit() {
        o = NULL;
        t = 1e38;
    }

    Hit(float _t, Object3D* _o, const Vector3f &n, const bool &_into) {
        t = _t;
        o = _o;
        normal = n.normalized();
        into = _into;
    }

    Hit(const Hit &h) {
        t = h.t;
        o = h.o;
        normal = h.normal;
        into = h.into;
    }

    // destructor
    ~Hit() = default;

    float getT() const {
        return t;
    }

    Object3D* getObject() const {
        return o;
    }

    Vector3f getNormal() const {
        return normal;
    }

    bool getInto() const {return into;}

    void set(float _t, Object3D* _o, const Vector3f &n, const bool &_into) {
        t = _t;
        o = _o;
        normal = n.normalized();
        into = _into;
    }

private:
    float t;
    Object3D* o;
    Vector3f normal;
    bool into = true;
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ", " << h.getInto() << ">";
    return os;
}

#endif // HIT_H
