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

    Hit(double _t, Object3D* _o, const Vector3d &n, const bool &_into) {
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

    double getT() const {
        return t;
    }

    Object3D* getObject() const {
        return o;
    }

    Vector3d getNormal() const {
        return normal;
    }

    bool getInto() const {return into;}

    void set(double _t, Object3D* _o, const Vector3d &n, const bool &_into) {
        t = _t;
        o = _o;
        normal = n.normalized();
        into = _into;
    }

private:
    double t;
    Object3D* o;
    Vector3d normal;
    bool into = true;
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ", " << h.getInto() << ">";
    return os;
}

#endif // HIT_H
