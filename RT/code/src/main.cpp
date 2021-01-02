
#include <bits/stdc++.h>

#include "parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "material.hpp"
#include "vecmath/vecmath.h"

#include <omp.h>

int samps, maxDep;
double filmGamma;
Group* baseGroup;
std::vector <Object3D*> lights;

inline double clamp(const double &x) {return x < 0 ? 0 : (x > 1 ? 1 : x);}
inline double gammaCorrection(const double &x) {return pow(clamp(x), 1.0 / filmGamma);}
inline Vector3d gammaCorrection(const Vector3d &v) {
    return Vector3d(gammaCorrection(v.x()), gammaCorrection(v.y()), gammaCorrection(v.z()));
}

inline Vector3d sampleHemisphereCos(const Vector3d &n, unsigned short *Xi) {
    double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
    Vector3d u = Vector3d::cross(fabs(n.x()) > 0.1 ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0), n).normalized();
    Vector3d v = Vector3d::cross(n, u);
    return (u * cos(r1) * r2s + v * sin(r1) * r2s + n * sqrt(1 - r2)).normalized();
}

// Forward
Vector3d rayTracing(const Ray &r, int dep, unsigned short *Xi, const bool &returnAtLight = false);

inline Vector3d diffuseRadiance(const Vector3d &x, const Vector3d &n, int dep, unsigned short *Xi) {
    Vector3d illum = Vector3d::ZERO;
    for (auto l: lights) {
        Vector3d y, ny;
        double A;
        if (!l->getSample(x, y, ny, A, Xi))
            continue;
        Vector3d z = (y - x).normalized();
        double cosTh = Vector3d::dot(n, z);
        if (cosTh <= 0)
            continue;
        double cosThP = Vector3d::dot(ny, -z);
        double dist = (x - y).length();
        Ray testRay(x, z);
        Hit testHit = Hit(dist + 1e-9, NULL, Vector3d::ZERO, true);
        if (baseGroup->intersect(testRay, testHit, 1e-9, true))
            continue;
        illum += l->getEmmision() * cosTh * cosThP / (y - x).squaredLength() * A;
    }
    return illum / M_PI + rayTracing(Ray(x, sampleHemisphereCos(n, Xi)), dep, Xi, true);
}

Vector3d rayTracing(const Ray &r, int dep, unsigned short *Xi, const bool &returnAtLight) {
    Hit hit = Hit();
    bool isIntersect = baseGroup->intersect(r, hit, 1e-9);
    if (!isIntersect)
        return Vector3d::ZERO;
    Object3D *o = hit.getObject();
    Material *m = o->getMaterial();
    bool into = hit.getInto();
    Vector3d e = Vector3d::ZERO, f = Vector3d::ZERO, g = m->getTran();
    if (into || m->getTwoSided()) f = m->getRefl();
    if (into) e = o->getEmmision();
    if (e != Vector3d::ZERO)
        if (returnAtLight) return Vector3d::ZERO;
        else return e;
    Vector3d x = r.pointAtParameter(hit.getT()),
             n = hit.getNormal();
    double p = std::min(0.99, std::fmax(std::fmax(f.x(), f.y()), f.z()));
    if (++dep > maxDep)
        if (erand48(Xi) < p) {f *= 1 / p; g *= 1 / p;}
        else return Vector3d::ZERO;
    if (f == Vector3d::ZERO)
        return Vector3d::ZERO;
    Material::SurfaceType type = m->getType();
    if (type == Material::DIFFUSE)
        return f * diffuseRadiance(x, n, dep, Xi);
    
    // Reflect
    Vector3d rD = r.getDirection();
    Ray reflRay(x, rD - 2 * Vector3d::dot(rD, n) * n);
    if (type == Material::CONDUCTOR)
        return f * rayTracing(reflRay, dep, Xi);
    
    // Fresnel
    double intIor = m->getIntIor(), extIor = m->getExtIor();
    if (!into) std::swap(intIor, extIor);
    double nnt = extIor / intIor, ddn = Vector3d::dot(rD, n),
            cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
    if (cos2t < 0)
        return f * rayTracing(reflRay, dep, Xi);
    double a = intIor - extIor, b = intIor + extIor, R0 = a * a / (b * b);

    if (type == Material::DIELECTRIC) {
        Vector3d refrDir = (rD * nnt - n * (ddn * nnt + sqrt(cos2t))).normalized();
        double c = 1 + (into ? ddn : Vector3d::dot(n, refrDir));
        double Re = R0 + (1 - R0) * pow(c, 5);
        double Tr = 1 - Re;
        double P = 0.25 + 0.5 * Re;
        double RP = Re / P, TP = Tr / (1 - P);
        if (dep > std::min(5.0, 0.4 * maxDep))
            if (erand48(Xi) < P)
                return f * rayTracing(reflRay, dep, Xi) * RP; // only reflect
            else
                return g * rayTracing(Ray(x, refrDir), dep, Xi) * TP; // only transmit
        else
            return f * rayTracing(reflRay, dep, Xi) * Re
                 + g * rayTracing(Ray(x, refrDir), dep, Xi) * Tr;
    }
    if (type == Material::PLASTIC) {
        double c = 1 + ddn;
        double Re = R0 + (1 - R0) * pow(c, 5);
        double Tr = 1 - Re;
        double P = 0.25 + 0.5 * Re;
        double RP = Re / P, TP = Tr / (1 - P);
        if (dep > std::min(5.0, 0.4 * maxDep))
            if (erand48(Xi) < P)
                return f * rayTracing(reflRay, dep, Xi) * RP; // only specular reflect
            else
                return g * diffuseRadiance(x, n, dep, Xi) * TP; // only diffuse
        else
            return f * rayTracing(reflRay, dep, Xi) * Re
                 + g * diffuseRadiance(x, n, dep, Xi) * Tr;
    }
}

int main(int argc, char *argv[]) {
    double timeStamp = omp_get_wtime();

    Parser parser(argv[1]);
    Camera *camera = parser.getCamera();
    baseGroup = parser.getGroup();
    lights = parser.getLights();

    int w = camera->getWidth(), h = camera->getHeight();
    Image renderedImg(w, h);

    samps = parser.getSampleCount() / 4;
    maxDep = parser.getMaxDep();
    filmGamma = parser.getGamma();

    #pragma omp parallel for collapse(1) schedule(dynamic, 1)
    for (int y = 0; y < h; ++y) {
        double load = 1.0 * y / h, t = omp_get_wtime() - timeStamp;
        fprintf(stderr, "\r %5.2lf%%\t\tUsed time: %5.2lf sec\t\tRemaining time: %5.2lf sec", 100 * load, t, t / load * (1 - load));
        unsigned short Xi[3] = {0, 0, (unsigned short) (y * y * y)};
        for (int x = 0; x < w; ++x) {
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
            renderedImg.SetPixel(x, y, gammaCorrection(finalColor));
        }
    }
    renderedImg.SaveImage(argv[2]);
    fprintf(stderr, "\nfinished, time = %5.2lf sec\n", omp_get_wtime() - timeStamp);
    return 0;
}