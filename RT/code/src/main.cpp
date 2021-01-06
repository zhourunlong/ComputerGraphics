#include <bits/stdc++.h>

#include "parser.hpp"
#include "image/image.hpp"
#include "camera.hpp"
#include "object/group.hpp"
#include "bsdf/bsdf.h"
#include "vecmath/vecmath.h"
#include "sampler.hpp"
#include "utils.hpp"
#include "emitter.hpp"

#include <omp.h>

int samps, maxDep;
double filmGamma;
Group* baseGroup;
std::vector <Object3D*> lights;
std::vector <Emitter*> emitters;

inline Vector3d emission(const Ray &ray) {
    Vector3d illum = Vector3d::ZERO;
    for (auto l: emitters)
        illum += l->radiance(ray);
    return illum;
}

inline Vector3d lightSampling(Material* m, const Vector3d &x,
    Hit& hit, const Vector3d &wo, Sampler* sampler) {

    Vector3d illum = Vector3d::ZERO;
    for (auto l: lights) {
        Vector3d y, ny;
        double A;
        if (!l->getSample(x, y, ny, A, sampler))
            continue;
        Vector3d z = (y - x).normalized();
        double cosTh = Vector3d::dot(hit.getShadeNormal(), z);
        if (cosTh <= 0)
            continue;
        Vector3d color = m->getColor(wo, z, hit);
        if (color.length() < 1e-9) continue;
        double cosThP = Vector3d::dot(ny, -z);
        double dist = (x - y).length();
        Ray testRay(x, z);
        Hit testHit = Hit(dist + 1e-9, NULL, NULL, Vector3d::ZERO, Vector2d::ZERO,
                          true);
        if (baseGroup->intersect(testRay, testHit, 1e-9, true))
            continue;
        illum += l->getEmission() * color * cosTh * cosThP
               / (y - x).squaredLength() * A;
    }
    return illum;
}

Vector3d rayTracing(Ray r, Sampler* sampler) {
    Vector3d f = Vector3d(1), res = Vector3d::ZERO;
    bool lastDiffuse = false;
    
    for (int dep = 0; dep < maxDep; ++dep) {
        Hit hit = Hit();
        bool isIntersect = baseGroup->intersect(r, hit, 1e-9);
        if (!isIntersect) {
            res += f * emission(r);
            break;
        }

        Object3D *o = hit.getObject();
        Material *m = hit.getMaterial();
        bool into = hit.getInto();
        Vector3d e = Vector3d::ZERO;
        if (into && (e = hit.getEmission()) != Vector3d::ZERO)  {
            if (!lastDiffuse || !hit.isSampleable()) res += f * e;
            break;
        }

        Vector3d x = r.pointAtParameter(hit.getT());
        if (m->needLightSampling())
            res += f * lightSampling(m, x, hit, -r.getDirection(), sampler);

        Vector3d d, g;
        m->sampleBSDF(-r.getDirection(), d, hit, g, sampler, lastDiffuse);
        f = abs(Vector3d::dot(d, hit.getShadeNormal())) * g * f;

        if (f.length() < 1e-9) break;
        double P = max(max(f.x(), f.y()), f.z());
        if (P < 0.05) {
            if (sampler->sampleDouble() > P) break;
            f = f / P;
        }

        r = Ray(x, d);
    }

    return res;
}

int main(int argc, char *argv[]) {
    double timeStamp = omp_get_wtime();

    Parser parser(argv[1]);
    Camera *camera = parser.getCamera();
    baseGroup = parser.getGroup();
    //baseGroup->print();
    lights = parser.getLights();
    emitters = parser.getEmitters();

    fprintf(stderr, "in total %d basic elements (triangles, spheres)\n",
        baseGroup->numObjects());

    int w = camera->getWidth(), h = camera->getHeight();
    Image renderedImg(w, h);

    samps = parser.getSampleCount() / 4;
    maxDep = parser.getMaxDep();
    filmGamma = parser.getGamma();

    fprintf(stderr, "started rendering\n");
    #pragma omp parallel for collapse(1) schedule(dynamic, 1) num_threads(90)
    for (int y = 0; y < h; ++y) {
    //for (int y = 660; y < 730; ++y) {
        unsigned short Xi[3] = {0, 0, (unsigned short) (y * y * y)};
        Sampler* sampler = new Sampler(Xi);
        for (int x = 0; x < w; ++x) {
        //for (int x = 430; x < 550; ++x) {
            Vector3d finalColor(0);
            for (int sy = -1; sy < 2; sy += 2)
                for (int sx = -1; sx < 2; sx += 2) {
                    Vector3d r(0);
                    for (int s = 0; s < samps; ++s) {
                        double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
                        double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
                        Ray camRay = camera->generateRay(Vector2d(x + 0.25 * sx + 0.5 * dx + 0.5, y + 0.25 * sy + 0.5 * dy + 0.5));
                        //Ray camRay = camera->generateRay(Vector2d(x, y));
                        r = r + rayTracing(camRay, sampler) / samps;
                    }
                    finalColor = finalColor + Vector3d(clamp(r.x()), clamp(r.y()), clamp(r.z())) / 4;
                }
            //std::cerr << x << " " << y << " " << finalColor << "\n";
            renderedImg.SetPixel(x, y, gammaCorrection(finalColor, filmGamma));
        }
        delete sampler;

        double load = 1.0 * (y + 1) / h, t = omp_get_wtime() - timeStamp;
        fprintf(stderr, "%5.2lf%%\t\tUsed time: %5.2lf sec\t\tRemaining time: %5.2lf sec\n", 100 * load, t, t / load * (1 - load));
    }
    renderedImg.SaveImage(argv[2]);
    fprintf(stderr, "finished rendering, time = %5.2lf sec\n", omp_get_wtime() - timeStamp);

/*
    int x = 149, y = 239;
    unsigned short Xi[3] = {0, 0, (unsigned short) (y * y * y)};
    Sampler* sampler = new Sampler(Xi);
    Ray camRay = camera->generateRay(Vector2d(x, y));
    rayTracing(camRay, sampler);
    delete sampler;
*/
    return 0;
}