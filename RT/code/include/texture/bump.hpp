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
        int w = bitmap->w,
            h = bitmap->h;
        Vector2d texCoor = hit.getTexCoor();
        int x = (int(texCoor.x() * w) % w + w) % w,
            y = (int(texCoor.y() * h) % h + h) % h;
        if (x == 0 || x == w - 1 || y == 0 || y == h - 1) {
            nestedBitmap->albedo(hit);
            return;
        }
        double bu = (bitmap->getPixel(x + 1, y, 0)
                  - bitmap->getPixel(x - 1, y, 0)) / 2.0,
               bv = (bitmap->getPixel(x, y + 1, 0)
                  - bitmap->getPixel(x, y - 1, 0)) / 2.0;
        Vector3d pu, pv, n;
        hit.getTangent(pu, pv);
        n = hit.getShadeNormal()
          + Vector3d::cross(hit.getShadeNormal(), bu * pv - bv * pu);
        //std::cerr << "bubv = " << bu << " " << bv << "\n";
        //std::cerr << "pupv = " << pu << " " << pv << "\n";
        //std::cerr << hit.getShadeNormal() << " " << n.normalized() << "\n";
        hit.setShadeNormal(n.normalized());
        nestedBitmap->albedo(hit);
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