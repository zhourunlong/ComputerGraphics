#pragma once

#include "texture.h"

inline Texture::Texture(const Vector3d &_uniform) {
    use_uniform = true;
    use_bitmap = false;
    uniform = _uniform;
    bitmap = NULL;
}

inline Texture::Texture(ClmbsImg_Data* _bitmap) {
    setBitmap(_bitmap);
}

inline void Texture::setBitmap(ClmbsImg_Data* _bitmap) {
    use_uniform = false;
    use_bitmap = true;
    bitmap = _bitmap;
}

inline ClmbsImg_Data* Texture::getBitmap() {return bitmap;}

inline void Texture::albedo(Hit &hit) {
    if (use_uniform) {
        hit.setColor(uniform);
        return;
    }
    
    int w = bitmap->w, h = bitmap->h;
    Vector2d texCoor = hit.getTexCoor();
    int x = (int(texCoor.x() * w) % w + w) % w,
        y = (int(texCoor.y() * h) % h + h) % h;
    int r = bitmap->data[(y * bitmap->w + x) * bitmap->bpp],
        g = bitmap->data[(y * bitmap->w + x) * bitmap->bpp + 1],
        b = bitmap->data[(y * bitmap->w + x) * bitmap->bpp + 2];
    hit.setColor(Vector3d(r / 256.0, g / 256.0, b / 256.0));
}

inline void Texture::print() {
    if (use_uniform)
        std::cout << "uniform, " << uniform << "\n";
    if (use_bitmap) {
        std::cout << "use bitmap at " << bitmap << "\n";
    }
}
