#pragma once

#include "ray.hpp"
#include "hit.h"
#include "hit.cpp"
#include "../vecmath/vecmath.h"
#include "../utils.hpp"
#include "../sampler.hpp"
#include "parser.hpp"
#include <bits/stdc++.h>

class Material;
class Hit;

typedef std::pair <double, double> query;

extern std::map <std::string, Material*> materialMap;

struct BoundPlane {
    double coorMin, coorMax;

    inline query queryIntersectX(Ray r) {
        Vector3d p = r.getOrigin(), v = r.getDirection();
        if (v.x() == 0) {
            if (p.x() < coorMin || p.x() > coorMax) return std::make_pair(0, -1);
            return std::make_pair(-1e38, 1e38);
        }
        double t0 = (coorMin - p.x()) / v.x(), t1 = (coorMax - p.x()) / v.x();
        if (t0 > t1) return std::make_pair(t1, t0);
        return std::make_pair(t0, t1);
    }

    inline query queryIntersectY(Ray r) {
        Vector3d p = r.getOrigin(), v = r.getDirection();
        if (v.y() == 0) {
            if (p.y() < coorMin || p.y() > coorMax) return std::make_pair(0, -1);
            return std::make_pair(-1e38, 1e38);
        }
        double t0 = (coorMin - p.y()) / v.y(), t1 = (coorMax - p.y()) / v.y();
        if (t0 > t1) return std::make_pair(t1, t0);
        return std::make_pair(t0, t1);
    }

    inline query queryIntersectZ(Ray r) {
        Vector3d p = r.getOrigin(), v = r.getDirection();
        if (v.z() == 0) {
            if (p.z() < coorMin || p.z() > coorMax) return std::make_pair(0, -1);
            return std::make_pair(-1e38, 1e38);
        }
        double t0 = (coorMin - p.z()) / v.z(), t1 = (coorMax - p.z()) / v.z();
        if (t0 > t1) return std::make_pair(t1, t0);
        return std::make_pair(t0, t1);
    }

    inline void merge(const BoundPlane &b) {
        coorMin = std::min(coorMin, b.coorMin);
        coorMax = std::max(coorMax, b.coorMax);
    }
};

// Base class for all 3d entities.
class Object3D {

public:

    enum ObjectType {
        SPHERE,
        TRIANGLE,
        MESH,
        GROUP,
        TRANSFORM
    };

    Object3D() : material(nullptr) {}

    ~Object3D() = default;

    explicit Object3D(Material *material) {
        this->material = material;
    }

    inline virtual std::string getMatRef() {return ref;}
    inline void setMatRef(const std::string &_ref) {ref = _ref;}

    inline virtual Material* getMaterial() {return material;}
    inline virtual void setMaterial(Material* _material) {material = _material;}

    inline virtual Vector3d getEmmision() {return emmision;}
    inline void setEmmision(const Vector3d &_emmision) {emmision = _emmision;}

    inline bool getNeedTransform() {return needTransform;}
    inline void setNeedTransform(const bool &_needTransform) {needTransform = _needTransform;}

    inline ObjectType getObjType() {return objType;}

    // Intersect Ray with this object. If hit, store information in hit structure.
    inline virtual bool intersect(const Ray &r, Hit &h, const double &tmin, const bool &testLs = false) = 0;

    inline virtual bool getSample(const Vector3d &x, Vector3d &y, Vector3d &ny, double &A, Sampler *Xi) {}

    inline virtual BoundPlane getBoundPlaneX() = 0;
    inline virtual BoundPlane getBoundPlaneY() = 0;
    inline virtual BoundPlane getBoundPlaneZ() = 0;

    inline virtual void print() = 0;

    inline virtual void finish() {}

protected:

    std::string ref = "";
    Material* material;
    Vector3d emmision = Vector3d::ZERO;
    bool needTransform = false;
    ObjectType objType;

    struct TreeNode {
        TreeNode *lc, *rc;
        BoundPlane planeX, planeY, planeZ;
        Object3D* obj;
    } *rt = NULL;

    void buildTree(TreeNode *&rt, std::vector <Object3D*> v, int dep) {
        rt = new TreeNode;
        rt->lc = rt->rc = NULL;
        if (v.size() == 1) {
            rt->planeX = v[0]->getBoundPlaneX();
            rt->planeY = v[0]->getBoundPlaneY();
            rt->planeZ = v[0]->getBoundPlaneZ();
            rt->obj = v[0];
            return;
        }

        std::vector <std::pair <double, int> > coorBuf;
        if (dep == 0) {
            for (int i = 0; i < v.size(); ++i)
                coorBuf.push_back(std::make_pair(v[i]->getBoundPlaneX().coorMin, i));
        } else if (dep == 1) {
            for (int i = 0; i < v.size(); ++i)
                coorBuf.push_back(std::make_pair(v[i]->getBoundPlaneY().coorMin, i));
        } else {
            for (int i = 0; i < v.size(); ++i)
                coorBuf.push_back(std::make_pair(v[i]->getBoundPlaneZ().coorMin, i));
        }
        std::sort(coorBuf.begin(), coorBuf.end());
        
        std::vector <Object3D*> tempV;
        for (int i = 0; i < v.size() / 2; ++i)
            tempV.push_back(v[coorBuf[i].second]);
        buildTree(rt->lc, tempV, (dep + 1) % 3);
        
        tempV.clear();
        for (int i = v.size() / 2; i < v.size(); ++i)
            tempV.push_back(v[coorBuf[i].second]);
        buildTree(rt->rc, tempV, (dep + 1) % 3);
        
        rt->planeX = rt->lc->planeX; rt->planeX.merge(rt->rc->planeX);
        rt->planeY = rt->lc->planeY; rt->planeY.merge(rt->rc->planeY);
        rt->planeZ = rt->lc->planeZ; rt->planeZ.merge(rt->rc->planeZ);
        rt-> obj = NULL;
    }

    bool queryIntersect(TreeNode *rt, const Ray &r, Hit &h, const double &tmin, const bool &testLs = false) {
        if (rt == NULL) return false;
        if (rt->obj != NULL) return rt->obj->intersect(r, h, tmin, testLs);
        query qX = rt->planeX.queryIntersectX(r),
              qY = rt->planeY.queryIntersectY(r),
              qZ = rt->planeZ.queryIntersectZ(r);
        double L = std::max(qX.first, std::max(qY.first, qZ.first)),
               U = std::min(qX.second, std::min(qY.second, qZ.second));
        if (L > U || U < 0 || L >= h.getT()) return false;
        bool ret = queryIntersect(rt->lc, r, h, tmin, testLs);
        if (testLs && ret) return true;
        ret |= queryIntersect(rt->rc, r, h, tmin, testLs);
        return ret;
    }

    void deleteTree(TreeNode *&rt) {
        if (rt == NULL) return;
        deleteTree(rt->lc); deleteTree(rt->rc);
        delete(rt); rt = NULL;
    }
};
