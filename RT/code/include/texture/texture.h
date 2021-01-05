#pragma once

#include <bits/stdc++.h>
#include "../vecmath/vecmath.h"
#include "../image/ClmbsImg.hpp"
#include "../hit.h"

class Texture {
public:
    Texture() {}

    inline Texture(const Vector3d &_uniform);

    inline Texture(ClmbsImg_Data* _bitmap);

    inline virtual void setBitmap(ClmbsImg_Data* _bitmap);
    inline ClmbsImg_Data* getBitmap();

    inline virtual void albedo(Hit &hit);

    inline void print();

protected:

    bool use_uniform, use_bitmap;
    Vector3d uniform;
    ClmbsImg_Data* bitmap = NULL;
};