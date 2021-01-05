#pragma once

#include <bits/stdc++.h>
#include "../vecmath/vecmath.h"
#include "../image/ClmbsImg.hpp"
#include "../hit.h"
#include "texture.h"

class Bump: public Texture {
public:
    
    Bump() {}

    inline Bump(ClmbsImg_Data* _bitmap) {setBitmap(_bitmap);}

    inline Bump(Bump* _b) {
        bitmap = _b->getBitmap();
    }

    inline void setNestedBitmap(Texture* _nestedBitmap) {
        nestedBitmap = _nestedBitmap;
    }

    inline void albedo(Hit &hit) override {
        nestedBitmap->albedo(hit);
        int w = bitmap->w,
            h = bitmap->h;
        Vector2d texCoor = hit.getTexCoor();
        int x = (int(texCoor.x() * w) % w + w) % w,
            y = (int(texCoor.y() * h) % h + h) % h;
        if (x == 0 || x == w - 1 || y == 0 || y == h - 1)
            return;
        double bu = (bitmap->getPixel(x + 1, y)
                  - bitmap->getPixel(x - 1, y)) / 2.0 / 256,
               bv = (bitmap->getPixel(x, y + 1)
                  - bitmap->getPixel(x, y - 1)) / 2.0 / 256;
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

private:

    Texture* nestedBitmap = NULL;
};