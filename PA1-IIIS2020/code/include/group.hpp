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
        v.clear();
    }

    explicit Group (int num_objects) {
        v.clear();
        v.resize(num_objects);
    }

    ~Group() override {
        for (int i = 0; i < v.size(); ++i) delete v[i];
        v.clear();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool ans = false;
        for (int i = 0; i < v.size(); ++i)
            ans |= v[i]->intersect(r, h, tmin);
        return ans;
    }

    void addObject(int index, Object3D *obj) {
        assert(index >= 0 && index < v.size() && v[index] == 0);
        v[index] = obj;
    }

    int getGroupSize() {
        return v.size();
    }

private:
    std::vector <Object3D*> v;
};

#endif
	
