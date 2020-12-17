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

    Group (int num_objects) {
        v.clear();
        v.resize(num_objects);
        plane.clear();
        //deleteTree(rt);
    }

    bool intersect(const Ray &r, Hit &h, double tmin) override {
        bool ans = false;
        for (int i = 0; i < plane.size(); ++i)
            ans |= plane[i]->intersect(r, h, tmin);
        ans |= queryIntersect(rt, r, h, tmin);
        return ans;
    }

    void addObject(int index, Object3D *obj) {
        assert(index >= 0 && index < v.size() && v[index] == 0);
        v[index] = obj;
        if (index == v.size() - 1)
            finish();
    }

    void addObject(Object3D *obj) {v.push_back(obj);}

    int getGroupSize() {return v.size();}

    Object3D* getObj(int idx) {
        assert(0 <= idx && idx < v.size());
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

    void print() override {
        std::cout << "===========================\n";
        std::cout << "=          Group          =\n";
        std::cout << "===========================\n";
        for (int i = 0; i < v.size(); ++i)
            v[i]-> print();
        std::cout << "---------------------------\n";
        std::cout << "-       End of Group      -\n";
        std::cout << "---------------------------\n";
    }

    void finish() override {
        std::vector <Object3D*> withoutPlane;
        for (int i = 0; i < v.size(); ++i) {
            v[i]->finish();
            if (v[i]->getIsPlane())
                plane.push_back(v[i]);
            else
                withoutPlane.push_back(v[i]);
        }
        if (withoutPlane.size()) buildTree(rt, withoutPlane, 0);
        else isPlane = true;
    }

private:

    std::vector <Object3D*> v, plane;
};

#endif
	
