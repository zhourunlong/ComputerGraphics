#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>

const int Dx[4] = {-1, 0, 1, 0}, Dy[4] = {0, 1, 0, -1};

class Element {
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override {
        int dx = xB - xA, dy = yB - yA, e, x, y, sgn;
        if (abs(dx) >= abs(dy)) {
            if (xA > xB) {
                std::swap(xA, xB); dx = -dx;
                std::swap(yA, yB); dy = -dy;
            }
            e = -dx; x = xA; y = yA;
            sgn = dy >= 0 ? 1 : -1; dy = std::abs(dy);
            for (int i = 0; i <= dx; ++i) {
                img.SetPixel(x, y, color);
                ++x; e += 2 * dy;
                if (e >= 0) {y += sgn; e -= 2 * dx;}
            }
        } else {
            if (yA > yB) {
                std::swap(xA, xB); dx = -dx;
                std::swap(yA, yB); dy = -dy;
            }
            e = -dy; x = xA; y = yA;
            sgn = dx >= 0 ? 1 : -1; dx = std::abs(dx);
            for (int i = 0; i <= dy; ++i) {
                img.SetPixel(x, y, color);
                ++y; e += 2 * dx;
                if (e >= 0) {x += sgn; e -= 2 * dy;}
            }
        }
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
                color.x(), color.y(), color.z());
    }
};

class Circle : public Element {

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override {
        int x, y = radius, e = 5 - 4 * radius;
        while (x <= y) {
            img.SetPixel(cx + x, cy + y, color);
            img.SetPixel(cx + y, cy + x, color);
            img.SetPixel(cx - x, cy + y, color);
            img.SetPixel(cx - y, cy + x, color);
            img.SetPixel(cx + x, cy - y, color);
            img.SetPixel(cx + y, cy - x, color);
            img.SetPixel(cx - x, cy - y, color);
            img.SetPixel(cx - y, cy - x, color);
            if (e < 0) e += 8 * x + 12;
            else {
                e += 8 * (x - y) + 20;
                --y;
            }
            ++x;
        }
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
               color.x(), color.y(), color.z());
    }
};

class Fill : public Element {

public:
    int cx, cy;
    Vector3f color;
    std::queue <std::pair <int, int> > Q;
    void draw(Image &img) override {
        int w = img.Width(), h = img.Height();
        Vector3f oldcolor = img.GetPixel(cx, cy);
        std::pair <int, int> u = std::make_pair(cx, cy);
        Q.push(u);
        while (!Q.empty()) {
            u = Q.front(); Q.pop();
            int x = u.first, y = u.second;
            if (img.GetPixel(x, y) == oldcolor) {
                img.SetPixel(x, y, color);
                for (int k = 0; k < 4; ++k) {
                    int tx = x + Dx[k], ty = y + Dy[k];
                    if (tx < 0 || tx >= w || ty < 0 || ty >= h) continue;
                    Q.push(std::make_pair(tx, ty));
                }
            }
        }
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
                color.x(), color.y(), color.z());
    }
};
