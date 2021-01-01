#pragma once

#include "object3d.hpp"
#include "ray.hpp"
#include "hit.h"
#include "hit.cpp"
#include <iostream>
#include <vector>

class Group : public Object3D {

public:

    inline Group() {
        objType = GROUP;
        v.clear();
        //deleteTree(rt);
    }

    inline Group (int num_objects) {
        objType = GROUP;
        v.clear();
        v.resize(num_objects);
        //deleteTree(rt);
    }

    inline bool intersect(const Ray &r, Hit &h, const double &tmin, const bool &testLs = false) override {
        return queryIntersect(rt, r, h, tmin, testLs);
    }

    inline void addObject(Object3D *obj) {v.push_back(obj);}

    inline int getGroupSize() {return v.size();}

    inline Object3D* getObj(int idx) {
        assert(0 <= idx && idx < v.size());
        return v[idx];
    }

    inline BoundPlane getBoundPlaneX() override {
        if (rt != NULL) return rt->planeX;
    }
    inline BoundPlane getBoundPlaneY() override {
        if (rt != NULL) return rt->planeY;
    }
    inline BoundPlane getBoundPlaneZ() override {
        if (rt != NULL) return rt->planeZ;
    }

    inline void print() override {
        std::cout << "===========================\n";
        std::cout << "=          Group          =\n";
        std::cout << "===========================\n";
        for (int i = 0; i < v.size(); ++i)
            v[i]-> print();
        std::cout << "---------------------------\n";
        std::cout << "-       End of Group      -\n";
        std::cout << "---------------------------\n";
    }

    inline void finish() override {
        for (int i = 0; i < v.size(); ++i)
            v[i]->finish();
        buildTree(rt, v, 0);
    }

private:

    std::vector <Object3D*> v;
};
