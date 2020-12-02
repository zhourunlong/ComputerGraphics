#ifndef MESH_H
#define MESH_H

#include <vector>
#include "object3d.hpp"
#include "triangle.hpp"
#include "Vector2f.h"
#include "Vector3f.h"


class Mesh : public Object3D {

public:
    Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    std::vector<Vector3f> v; // point index pool
    std::vector<TriangleIndex> t; // point indices for each triangle
    std::vector<Vector3f> n; // normal vectors for each triangle
    bool intersect(const Ray &r, Hit &h, float tmin) {
        return queryIntersect(rt, r, h, tmin);
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

private:

    // Normal can be used for light estimation
    void computeNormal();
    std::vector <Object3D*> triangles;
};

#endif
