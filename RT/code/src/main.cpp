
#include <bits/stdc++.h>

#include "parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "object/group.hpp"
#include "bsdf/bsdf.h"
#include "vecmath/vecmath.h"
#include "sampler.hpp"
#include "utils.hpp"

#include <omp.h>

int samps, maxDep;
double filmGamma;
Group* baseGroup;
std::vector <Object3D*> lights;


// https://zhuanlan.zhihu.com/p/20119162
inline double CosTheta2(const Vector3d& vec) { return vec.z() * vec.z(); }
inline double CosTheta(const Vector3d& vec) { return vec.z(); }
inline double TanTheta(const Vector3d& vec) {
	double temp = 1 - vec.z() * vec.z();
	if (temp <= 0.0)
		return 0.0;
	return sqrt(temp) / vec.z();
}
inline double TanTheta2(const Vector3d& vec) {
	double temp = 1 - vec.z() * vec.z();
	if (temp <= 0.0)
		return 0.0;
	return temp / (vec.z() * vec.z());
}
inline double GGX_D(Vector3d wm, double alpha){
    double tanTheta2 = TanTheta2(wm);
	double cosTheta2 = CosTheta2(wm);

	double root = alpha / (cosTheta2 * (alpha * alpha + tanTheta2));

	return 1/M_PI * (root * root);
}
inline double Smith_G1(Vector3d v, Vector3d wm, double alpha) {
    if (wm.z() < 0) wm = -wm;

    double tanTheta = abs(TanTheta(v));

    if (tanTheta == 0.0)
        return 1.0;

    if (Vector3d::dot(v, wm) * CosTheta(v) <= 0)
        return 0.0;

    double root = alpha * tanTheta;
    return 2.0 / (1 + sqrt(1 + root*root));
}
inline double Smith_G(Vector3d wo, Vector3d wi, Vector3d wm, double alpha) {
	return Smith_G1(wo, wm, alpha) * Smith_G1(wi, wm, alpha);
}

// https://schuttejoe.github.io/post/ggximportancesamplingpart2/
Vector3d GgxVndf(Vector3d wo, double roughness, double u1, double u2) {
    float ttt = 1;
    if (wo.z() < 0) {
        wo.z() = -wo.z();
        ttt = -1;
    }
    Vector3d v = Vector3d(wo.x() * roughness,
                          wo.y() * roughness,
                          wo.z()).normalized();
    Vector3d t1 = Vector3d::cross(fabs(v.x()) > 0.1 ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0), v).normalized();
    Vector3d t2 = Vector3d::cross(t1, v);
    double a = 1.0f / (1.0f + v.z());
    double r = sqrtf(u1);
    double phi = (u2 < a) ? (u2 / a) * M_PI 
                         : M_PI + (u2 - a) / (1.0f - a) * M_PI;
    double p1 = r * cos(phi);
    double p2 = r * sin(phi) * ((u2 < a) ? 1.0f : v.z());
    Vector3d n = p1 * t1 + p2 * t2
               + sqrt(max(0.0, 1.0 - p1 * p1 - p2 * p2)) * v;
    return Vector3d(roughness * n.x(),
                    roughness * n.y(),
                    max(0.0, n.z()) * ttt).normalized();
}

inline Vector3d lightSampling(Material* m, const Vector3d &x,
    const Hit& hit, const Vector3d &wo, Sampler* sampler) {

    Vector3d illum = Vector3d::ZERO;
    for (auto l: lights) {
        Vector3d y, ny;
        double A;
        if (!l->getSample(x, y, ny, A, sampler))
            continue;
        Vector3d z = (y - x).normalized();
        double cosTh = Vector3d::dot(hit.getNormal(), z);
        if (cosTh <= 0)
            continue;
        Vector3d color = m->getColor(wo, z, hit);
        if (color.length() < 1e-9) continue;
        double cosThP = Vector3d::dot(ny, -z);
        double dist = (x - y).length();
        Ray testRay(x, z);
        Hit testHit = Hit(dist + 1e-9, NULL, Vector3d::ZERO, true);
        if (baseGroup->intersect(testRay, testHit, 1e-9, true))
            continue;
        illum += l->getEmmision() * color * cosTh * cosThP
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
        if (!isIntersect) break;

        //std::cerr << r << " " << r.pointAtParameter(hit.getT()) << "\n";
        Object3D *o = hit.getObject();
        Material *m = o->getMaterial();
        bool into = hit.getInto();
        Vector3d e;
        if (into && (e = o->getEmmision()) != Vector3d::ZERO)  {
            if (!lastDiffuse) res += f * e;
            break;
        }

        Vector3d x = r.pointAtParameter(hit.getT());
        if (m->needLightSampling())
            res += f * lightSampling(m, x, hit, -r.getDirection(), sampler);

        Vector3d d, g;
        m->sampleBSDF(-r.getDirection(), d, hit, g, sampler, lastDiffuse);
        f = abs(Vector3d::dot(d, hit.getNormal())) * g * f;

        if (f.length() < 1e-9) break;
        double P = max(max(f.x(), f.y()), f.z());
        if (P < 0.05) {
            if (sampler->sampleDouble() > P) break;
            f = f / P;
        }

        r = Ray(x, d);
    }
    
    return res;

/*
    if (type == Material::ROUGHPLASTIC) {
        double c = 1 + ddn;
        double Re = R0 + (1 - R0) * pow(c, 5);
        double Tr = 1 - Re;
        double P = 0.25 + 0.5 * Re;
        double RP = Re / P, TP = Tr / (1 - P);
        /*
        double F = Fresnel(abs(ddn), nnt);
        // 1 / nnt?
        double alpha = m->getAlpha();

        Vector3d u = Vector3d::cross(fabs(n.x()) > 0.1 ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0), n).normalized();
        Vector3d v = Vector3d::cross(n, u);
    
        Vector3d wo = Vector3d(Vector3d::dot(wo, u),
                               Vector3d::dot(wo, v),
                               Vector3d::dot(wo, n));
        Vector3d wm = GgxVndf(wo, alpha, erand48(Xi), erand48(Xi));
        Vector3d wi = 2 * Vector3d::dot(wm, wo) * wm - wo;
        wm = (wo + wi).normalized();
        double pdf = F * Smith_G1(wi, wm, alpha)
                   * GGX_D(wm, alpha)
                   * abs(Vector3d::dot(wo, wm))
                   / (4 * abs(Vector3d::dot(wi, wm)) * abs(wo.z()));
        /*double brdf = F * GGX_D(wm, alpha)
                    * Smith_G(wo,wi,wm,alpha)
                    / abs(4 * wo.z() * wi.z());
        */
       /*
        Vector3d in = wi.x() * u + wi.y() * v + wi.z() * n;
        // multiply cos ? 
        if (dep > std::min(5.0, 0.4 * maxDep))
            if (erand48(Xi) < P)
                return 1.0 / pdf * f * rayTracing(Ray(x, in), dep, Xi) * RP; // only specular reflect
            else
                return g * diffuse
            case Material:) < P)
            return f * rayTracing(reflRay, dep, sampler) * RP; // only specular reflect
        else
            return g * diffuseRadiance(x, n, dep, sampler) * TP; // only diffuse
    }*/
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
        Sampler* sampler = new Sampler(Xi);
        for (int x = 0; x < w; ++x) {
            Vector3d finalColor(0);
            for (int sy = -1; sy < 2; sy += 2)
                for (int sx = -1; sx < 2; sx += 2) {
                    Vector3d r(0);
                    for (int s = 0; s < samps; ++s) {
                        double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
                        double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
                        Ray camRay = camera->generateRay(Vector2d(x + 0.25 * sx + 0.5 * dx + 0.5, y + 0.25 * sy + 0.5 * dy + 0.5));
                        r = r + rayTracing(camRay, sampler) / samps;
                    }
                    finalColor = finalColor + Vector3d(clamp(r.x()), clamp(r.y()), clamp(r.z())) / 4;
                }
            renderedImg.SetPixel(x, y, gammaCorrection(finalColor, filmGamma));
        }
        delete sampler;
    }
    renderedImg.SaveImage(argv[2]);
    fprintf(stderr, "\nfinished, time = %5.2lf sec\n", omp_get_wtime() - timeStamp);
/*
    int x = 565, y = 206;
    unsigned short Xi[3] = {0, 0, (unsigned short) (y * y * y)};
    Sampler* sampler = new Sampler(Xi);
    Ray camRay = camera->generateRay(Vector2d(x, y));
    rayTracing(camRay, sampler);
    delete sampler;
*/
    return 0;
}