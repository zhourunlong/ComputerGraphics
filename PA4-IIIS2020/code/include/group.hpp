#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>

class Group : public Object3D {
    std::vector<Object3D*> objects;

public:

    Group() {
        objects.clear();
    }

    explicit Group (int num_objects) {
        objects.resize(num_objects);
    }

    ~Group() override {
        for (auto* o : objects) {
            delete o;
        }
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool result = false;
        for (auto *obj : objects) {
            result |= obj->intersect(r, h, tmin);
        }
        return result;
    }

    void drawGL() override {
        for (auto *obj : objects) {
            obj->drawGL();
        }
    }

    void addObject(int index, Object3D *obj) {
        objects[index] = obj;
    }

    int getGroupSize() {
        return objects.size();
    }

    Object3D* getObject(int idx) const {
        return objects[idx];
    }

private:

};

#endif
