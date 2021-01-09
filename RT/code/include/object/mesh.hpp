#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>
#include "object3d.hpp"
#include "triangle.hpp"
#include "vecmath/vecmath.h"

class Mesh : public Object3D {

public:
    inline Mesh() {
        objType = MESH;
        triangles.clear();
        faceNormals = false;
    }
    
    inline void setFile(const char* filename) {
        // Optional: Use tiny obj loader to replace this simple one.
        std::ifstream f;
        f.open(filename);
        if (!f.is_open()) {
            std::cout << "Cannot open " << filename << "\n";
            return;
        }
        std::string line;
        std::string vTok("v");
        std::string fTok("f");
        std::string texTok("vt");
        std::string normTok("vn");
        char bslash = '/', space = ' ';
        std::string tok;
        while (true) {
            std::getline(f, line);
            if (f.eof()) {
                break;
            }
            if (line.size() < 3) {
                continue;
            }
            if (line.at(0) == '#') {
                continue;
            }
            std::stringstream ss(line);
            ss >> tok;
            if (tok == vTok) {
                Vector3d vec;
                ss >> vec[0] >> vec[1] >> vec[2];
                v.push_back(vec);
            } else if (tok == fTok) {
                // !! consider without tex corrdinate 
                if (line.find(bslash) != std::string::npos) {
                    std::replace(line.begin(), line.end(), bslash, space);
                    std::stringstream facess(line);
                    TriangleIndex trig, texId, normId;
                    facess >> tok;
                    for (int ii = 0; ii < 3; ii++) {
                        facess >> trig[ii] >> texId[ii] >> normId[ii];
                        --trig[ii]; --texId[ii]; --normId[ii];
                    }
                    t.push_back(trig); ve.push_back(normId); tx.push_back(texId);
                } else {
                    TriangleIndex trig;
                    for (int ii = 0; ii < 3; ii++) {
                        ss >> trig[ii];
                        trig[ii]--;
                    }
                    t.push_back(trig);
                }
            } else if (tok == texTok) {
                Vector2d tex;
                ss >> tex[0];
                ss >> tex[1];
                texCoor.push_back(tex);
            } else if (tok == normTok) {
                Vector3d norm;
                ss >> norm[0];
                ss >> norm[1];
                ss >> norm[2];
                verNorm.push_back(norm.normalized());
            }
        }
        f.close();

        for (int triId = 0; triId < t.size(); ++triId) {
            TriangleIndex& triIndex = t[triId];
            Triangle* tri = new Triangle(v[triIndex[0]], v[triIndex[1]], v[triIndex[2]], material);
            if (ve.size())
                tri->setVertexNormal(
                    verNorm[ve[triId][0]], verNorm[ve[triId][1]], verNorm[ve[triId][2]]);
            if (tx.size())
                tri->setTextureCoordinate(
                    texCoor[tx[triId][0]], texCoor[tx[triId][1]], texCoor[tx[triId][2]]);
            tri->calcTangent();
            triangles.push_back(tri);
        }

        buildTree(rt, triangles, 0);
    }

    inline void setTriangles(std::vector <Object3D*> _triangles) {
        triangles = _triangles;
        buildTree(rt, triangles, 0);
    }

    inline void setFaceNorm(bool _faceNormals) {
        if (_faceNormals)
            for (int i = 0; i < triangles.size(); ++i) {
                Triangle* tri = static_cast<Triangle*>(triangles[i]);
                tri->disableVertexNormal();
            }
    }

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    inline bool intersect(const Ray &r, Hit &h, const double &tmin, const bool &testLs = false) {
        bool ret = queryIntersect(rt, r, h, tmin, testLs);
        if (ret) {
            h.setObject(this);
            h.setMaterial(material);
            h.setSampleable(sampleable);
            h.setEmission(emission);
        }
        return ret;
    }

    inline void setEmission(const Vector3d &_emission) override {
        emission = _emission;
        sampleable = false;
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

    inline int numObjects() override {return triangles.size();}

    inline void print() override {
        std::cout << "===== Mesh =====\n";
        std::cout << t.size() << " triangles\n";
        std::cout << "material: " << ref << "\n";
        if (material) material->print();
        if (emission != Vector3d::ZERO)
            std::cout << "emission: " << emission << "\n";
        std::cout << "----------------\n";
    }

private:
    bool faceNormals = false;
    std::vector <Vector3d> v;              // point index pool
    std::vector <TriangleIndex> t, ve, tx; // point indices for each triangle
    std::vector <Vector3d> verNorm;        // vertex normals
    std::vector <Vector2d> texCoor;        // texture coordinate
    std::vector <Object3D*> triangles;
};
