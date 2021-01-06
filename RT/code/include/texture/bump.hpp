#pragma once

#include <bits/stdc++.h>
#include "../vecmath/vecmath.h"
#include "../image/ClmbsImg.hpp"
#include "../hit.h"
#include "texture.h"

class Bump: public Texture {
public:
    
    Bump() {}

    inline Bump(ClmbsImg_Data* _bitmap) {
        setBitmap(_bitmap);
        calculate();
    }

    inline Bump(Bump* _b) {
        setBitmap(_b->getBitmap());
        calculate();
    }

    inline void setNestedBitmap(Texture* _nestedBitmap) {
        nestedBitmap = _nestedBitmap;
    }

    inline void albedo(Hit &hit) override {
        nestedBitmap->albedo(hit);
        int w = bitmap->w, h = bitmap->h;
        Vector2d texCoor = hit.getTexCoor();
        int x = int(texCoor.x() * w),
            y = int(texCoor.y() * h);
        if (x <= 0 || x >= w || y < 0 || y >= h - 1)
            return;
        double xr = texCoor.x() * w - x, yr = texCoor.y() * h - y;
        double dfdx1 = dfdx[x][y], 
               dfdx2 = dfdx[x][y + 1],
               dfdy1 = dfdy[x][y],
               dfdy2 = dfdy[x - 1][y];
        double bu = xr * dfdx2 + (1 - xr) * dfdx1,
               bv = yr * dfdy2 + (1 - yr) * dfdy1;
        Vector3d pu, pv, n, shadeN = hit.getShadeNormal();
        hit.getTangent(pu, pv);
        n = (shadeN + Vector3d::cross(shadeN, bu * pv - bv * pu)).normalized();
        if (Vector3d::dot(n, shadeN) < 0) n = -n;
        hit.setShadeNormal(n);
        // https://web.eecs.umich.edu/~sugih/courses/eecs487/lectures/26-BumpMap+ProcTex.pdf
    }

    inline void print() {
        std::cout << "bumpmap, bump data at " << &bitmap << "\n";
        std::cout << "nested bitmap -----\n";
        nestedBitmap->print();
        std::cout << "----- nested bitmap\n";
    }

protected:

    void calculate() {
        int w = bitmap->w, h = bitmap->h;
        
        dfdx = new double*[w];
        dfdy = new double*[w];
        
        for (int x = 1; x < w; ++x) {
            dfdx[x] = new double[h];
            dfdy[x] = new double[h];
            for (int y = 0; y < h - 1; ++y) {
                dfdx[x][y] = (bitmap->getPixel(x - 1, y) - bitmap->getPixel(x, y)) / 256.0;
                dfdy[x][y] = (bitmap->getPixel(x, y + 1) - bitmap->getPixel(x, y)) / 256.0;
            }
        }
    }

    double** dfdx;
    double** dfdy;
    Texture* nestedBitmap = NULL;
};