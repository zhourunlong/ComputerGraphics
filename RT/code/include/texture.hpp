#pragma once

#include <bits/stdc++.h>
#include "vecmath/vecmath.h"
#include "ClmbsImg.hpp"
#include "hit.h"

class Texture {
public:
    enum FilterType {
        EWA,
        TRILINEAR,
        NEAREST
    };
    
    Texture() {}

    inline Texture(const Vector3d &_uniform) {
        use_uniform = true;
        use_bitmap = false;
        uniform = _uniform;
        bitmap = NULL;
    }

    inline Texture(ClmbsImg_Data* _bitmap) {
        setFile(_bitmap);
    }

    inline void setFile(ClmbsImg_Data* _bitmap) {
        use_uniform = false;
        use_bitmap = true;
        bitmap = _bitmap;
    }

    inline void setFilterType(const FilterType &_filterType) {
        filterType = _filterType;
    }

    inline Vector3d albedo(const Vector2d &texCoor) {
        if (use_uniform)
            return uniform;
        int w = bitmap->w, h = bitmap->h;
        int x = (int(texCoor.x() * w) % w + w) % w,
            y = ((-int(texCoor.y() * h) - 1) % h + h) % h;
        int r = bitmap->data[(y * bitmap->w + x) * bitmap->bpp],
            g = bitmap->data[(y * bitmap->w + x) * bitmap->bpp + 1],
            b = bitmap->data[(y * bitmap->w + x) * bitmap->bpp + 2];
        return Vector3d(r / 256.0, g / 256.0, b / 256.0);
    }

    inline void print() {
        if (use_uniform)
            std::cout << "uniform, " << uniform << "\n";
        if (use_bitmap) {
            std::cout << "use bitmap at " << bitmap << "\nfilter: ";
            switch (filterType) {
                case EWA: {std::cout << "ewa\n"; break;}
                case TRILINEAR: {std::cout << "trilinear\n"; break;}
                case NEAREST: {std::cout << "nearest\n"; break;}
            }
        }
    }

private:
    
    FilterType filterType = EWA;
    bool use_uniform, use_bitmap;
    Vector3d uniform;
    ClmbsImg_Data* bitmap = NULL;
};