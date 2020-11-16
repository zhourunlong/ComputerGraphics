#ifndef FFD_CAGE_HPP
#define FFD_CAGE_HPP

#include "object3d.hpp"
#include "mesh.hpp"
#include "curve.hpp"

class FFDCage : public Object3D {

    // The underlying mesh to be controlled.
    Mesh* geometry;

    // Last control point indices users edited.
    // Used for highlighting that point for better visualization.
    int lastUpdateX;
    int lastUpdateY;
    int lastUpdateZ;

    // Local parametrization (ui, vi, wi) of each vertex i on the mesh.
    std::vector<Vector3f> parametrization;

public:

    // Resolution of the control lattice.
    int resX;       // L + 1
    int resY;       // M + 1
    int resZ;       // N + 1
    // Positions of control points P_lmn
    std::vector<std::vector<std::vector<Vector3f>>> controls;

    FFDCage(Mesh* geom, int rx, int ry, int rz) : geometry(geom), resX(rx), resY(ry), resZ(rz),
        lastUpdateX(-1), lastUpdateY(-1), lastUpdateZ(-1) {
        initialize();
    }

    void initialize() {
        // Compute geometry bounding box.
        Vector3f bboxMin = Vector3f::INF_LARGE;
        Vector3f bboxMax = -Vector3f::INF_LARGE;
        for (const auto& v : geometry->v) {
            bboxMin.extendMin(v);
            bboxMax.extendMax(v);
        }
        Vector3f bboxDim = bboxMax - bboxMin;

        // Compute positions of each control points.
        controls.resize(resX + 1);
        for (int x = 0; x < resX + 1; ++x) {
            controls[x].resize(resY + 1);
            for (int y = 0; y < resY + 1; ++y) {
                controls[x][y].resize(resZ + 1);
                for (int z = 0; z < resZ + 1; ++z) {
                    controls[x][y][z] = Vector3f(
                            bboxMin.x() + bboxDim.x() * x / resX,
                            bboxMin.y() + bboxDim.y() * y / resY,
                            bboxMin.z() + bboxDim.z() * z / resZ);
                }
            }
        }

        // Compute Local Parametrization.
        parametrization.reserve(geometry->v.size());
        for (const auto& gv : geometry->v) {
            // TODO (PA4): 
        }
    }

    bool isEditable() override {
        return true;
    }

    void update() {
        // TODO (PA4): Update each vertex position in this->geometry according to positions of control points.
        for (int gi = 0; gi < (int) parametrization.size(); ++gi) {
            Vector3f& vertex = geometry->v[gi];
            
        }
    }

    void update(int x, int y, int z, const Vector3f& newPos) {
        assert(x >= 0 && x <= resX && y >= 0 && y <= resY && z >= 0 && z <= resZ);
        controls[x][y][z] = newPos;
        lastUpdateX = x; lastUpdateY = y; lastUpdateZ = z;
        update();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        return geometry->intersect(r, h, tmin);
    }

    void drawGL() override {
        Object3D::drawGL();
        geometry->drawGL();

        // Draw Lattice control point.
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glPointSize(5.0);
        glBegin(GL_POINTS);
        for (int x = 0; x <= resX; ++x) {
            for (int y = 0; y <= resY; ++y) {
                for (int z = 0; z <= resZ; ++z) {
                    if (x == lastUpdateX && y == lastUpdateY && z == lastUpdateZ) {
                        glColor3d(1.0, 0.0, 0.0);
                    } else {
                        glColor3d(1.0, 1.0, 1.0);
                    }
                    glVertex3fv(controls[x][y][z]);
                }
            }
        }
        glEnd();

        // Draw lines connecting lattice control points.
        glBegin(GL_LINES);
        glColor3d(1.0, 1.0, 1.0);
        for (int x = 0; x <= resX; ++x) {
            for (int y = 0; y <= resY; ++y) {
                for (int z = 0; z <= resZ; ++z) {
                    if (x != resX) {
                        glVertex3fv(controls[x][y][z]);
                        glVertex3fv(controls[x + 1][y][z]);
                    }
                    if (y != resY) {
                        glVertex3fv(controls[x][y][z]);
                        glVertex3fv(controls[x][y + 1][z]);
                    }
                    if (z != resZ) {
                        glVertex3fv(controls[x][y][z]);
                        glVertex3fv(controls[x][y][z + 1]);
                    }
                }
            }
        }
        glEnd();

        glPopAttrib();
    }
};

#endif // FFD_CAGE_HPP
