#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D {

public:

    Group() {
        v.clear(); plane.clear();
        //deleteTree(rt);
    }

    explicit Group (int num_objects) {
        v.clear();
        v.resize(num_objects);
        plane.clear();
        //deleteTree(rt);
    }

    ~Group() override {
        //deleteTree(rt);
        for (int i = 0; i < v.size(); ++i) delete v[i];
        v.clear();
        plane.clear();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool ans = false;
        for (int i = 0; i < plane.size(); ++i)
            ans |= plane[i]->intersect(r, h, tmin);
        ans |= queryIntersect(rt, r, h, tmin);
        return ans;
    }

    void addObject(int index, Object3D *obj) {
        assert(index >= 0 && index < v.size() && v[index] == 0);
        v[index] = obj;
        if (index == v.size() - 1) {
            std::vector <Object3D*> withoutPlane;
            for (int i = 0; i < v.size(); ++i)
                if (v[i]->testPlane())
                    plane.push_back(v[i]);
                else
                    withoutPlane.push_back(v[i]);
            if (withoutPlane.size()) buildTree(rt, withoutPlane, 0);
            else isPlane = true;
        }
    }

    int getGroupSize() {
        return v.size();
    }

    Object3D* getObject(int idx) const {
        return v[idx];
    }

    BoundPlane getBoundPlaneX() override {
        if (rt != NULL) return rt->planeX;
    }
    BoundPlane getBoundPlaneY() override {
        if (rt != NULL) return rt->planeY;
    }
    BoundPlane getBoundPlaneZ() override {
        if (rt != NULL) return rt->planeZ;
    }

    void drawGL() override {
        for (int i = 0; i < v.size(); ++i)
            v[i]->drawGL();
    }

private:

    std::vector <Object3D*> v, plane;
};

#endif
