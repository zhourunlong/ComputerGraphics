#include <bits/stdc++.h>

#include "parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "material.hpp"
#include "vecmath/vecmath.h"

#include <omp.h>

int samps, maxDep;
Group* baseGroup;

inline double clamp(double x) {return x < 0 ? 0 : (x > 1 ? 1 : x);}

inline int calcBlock(int n) {
    double r = 0;
    int b = 0;
    for (int a = 20; a < 40; ++a) {
        if (n % a == 0) return a;
        if (1.0 * (n % a) / a > r) {
            r = 1.0 * (n % a) / a;
            b = a;
        }
    }
    return b;
}

Vector3d rayTracing(const Ray &r, int dep, unsigned short *Xi) {
    Hit hit = Hit();
    bool isIntersect = baseGroup->intersect(r, hit, 1e-9);
    if (!isIntersect)
        return Vector3d(0);
    Object3D *o = hit.getObject();
    Material *m = o->getMaterial();
    Vector3d x = r.pointAtParameter(hit.getT()),
             n = hit.getNormal(),
             f = m->getRefl();
    double p = std::fmax(std::fmax(f.x(), f.y()), f.z());
    if (++dep > maxDep)
        if (erand48(Xi) < p) f *= 1 / p;
        else return o->getEmmision();
    Material::SurfaceType type = m->getType();
    if (type == Material::DIFF) {
        double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
        Vector3d w = n;
        Vector3d u = Vector3d::cross(fabs(w.x()) > 0.1 ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0), w).normalized();
        Vector3d v = Vector3d::cross(w, u);
        Vector3d d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalized();
        return o->getEmmision() + f * rayTracing(Ray(x, d), dep, Xi);
    }
    Vector3d rD = r.getDirection();
    Ray reflRay = Ray(x, rD - 2 * Vector3d::dot(rD, n) * n);
    if (type == Material::COND)
        return o->getEmmision() + f * rayTracing(reflRay, dep, Xi);
    if (type == Material::DIEL) {
        bool into = hit.getInto();
        double intIor = m->getIntIor(), extIor = m->getExtIor();
        if (!into) std::swap(intIor, extIor);
        double nnt = extIor / intIor, ddn = Vector3d::dot(rD, n),
               cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
        if (cos2t < 0)
            return o->getEmmision() + f * rayTracing(reflRay, dep, Xi);
        Vector3d refrDir = (rD * nnt - n * (ddn * nnt + sqrt(cos2t))).normalized(),
                 g = m->getTran();
        double a = intIor - extIor, b = intIor + extIor;
        double R0 = a * a / (b * b), c = 1 + (into ? ddn : Vector3d::dot(n, refrDir));
        double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re;
        double P = 0.25 + 0.5 * Re, RP = Re / P, TP = Tr / (1 - P);
        if (dep > std::min(5.0, 0.4 * maxDep))
            if (erand48(Xi) < P)
                return o->getEmmision() + f * rayTracing(reflRay, dep, Xi) * RP;
            else
                return o->getEmmision() + g * rayTracing(Ray(x, refrDir), dep, Xi) * TP;
        else
            return o->getEmmision() + f * rayTracing(reflRay, dep, Xi) * Re
                                    + g * rayTracing(Ray(x, refrDir), dep, Xi) * Tr;
    }
}

int main(int argc, char *argv[]) {
    double timeStamp = omp_get_wtime();
    int nthreads, rank;
    
    #pragma omp parallel
    {
        nthreads = omp_get_num_threads();
        rank = omp_get_thread_num();
        if (rank == 0)
            fprintf(stderr, "working on %d threads\n", nthreads);
    }

    Parser parser(argv[1]);
    Camera *camera = parser.getCamera();
    baseGroup = parser.getGroup();

    int w = camera->getWidth(), h = camera->getHeight();
    Image renderedImg(w, h);

    int a = calcBlock(w), b = calcBlock(h), numBlk = 0;
    int kX[(w / a + 2) * (h / b + 2)], kY[(w / a + 2) * (h / b + 2)];
    for (int y = 0; y < h; y += b)
        for (int x = 0; x < w; x += a) {
            kX[numBlk] = x;
            kY[numBlk] = y;
            ++numBlk;
        }
    
    fprintf(stderr, "%d x %d picture partitioned into %d %d x %d blocks\n", w, h, numBlk, a, b);

    samps = parser.getSampleCount() / 4;
    maxDep = parser.getMaxDep();
    int finBlk = 0;

    #pragma omp parallel for collapse(1) schedule(guided) shared(finBlk)
    for (int idx = 0; idx < numBlk; ++idx) {
        double load = 1.0 * finBlk / (numBlk - 1), t = omp_get_wtime() - timeStamp;
        fprintf(stderr, "\r %5.2lf%%\t\tUsed time: %5.2lf sec\t\tRemaining time: %5.2lf sec", 100 * load, t, t / load * (1 - load));
        int ey = std::min(h, kY[idx] + b), ex = std::min(w, kX[idx] + a);
        for (int y = kY[idx]; y < ey; ++y) {
            unsigned short Xi[3] = {0, 0, (unsigned short) (y * y * y)};
            for (int x = kX[idx]; x < ex; ++x) {
                Vector3d finalColor(0);
                for (int sy = -1; sy < 2; sy += 2)
                    for (int sx = -1; sx < 2; sx += 2) {
                        Vector3d r(0);
                        for (int s = 0; s < samps; ++s) {
                            double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
                            double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
                            Ray camRay = camera->generateRay(Vector2d(x + 0.25 * sx + 0.5 * dx + 0.5, y + 0.25 * sy + 0.5 * dy + 0.5));
                            r = r + rayTracing(camRay, 0, Xi) / samps;
                        }
                        finalColor = finalColor + Vector3d(clamp(r.x()), clamp(r.y()), clamp(r.z())) / 4;
                    }
                renderedImg.SetPixel(x, y, finalColor);
            }
        }
        #pragma omp critical
        {
            ++finBlk;
        }
    }

    renderedImg.SaveImage(argv[2]);
    return 0;
}

