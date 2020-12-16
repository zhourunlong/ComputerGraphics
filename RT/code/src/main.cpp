#include <bits/stdc++.h>

#include "parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"

using namespace std;

/*
int maxDep = 5;

Vector3f rayTracing(const Ray &r, int dep, bool &isIntersect, unsigned short *Xi) {
    Hit hit;
    isIntersect = baseGroup->intersect(r, hit, 0);
    if (!isIntersect)
        return Vector3f(0);
    Material *m = hit.getMaterial();
    Vector3f x = r.pointAtParameter(hit.getT()),
             n = hit.getNormal(),
             f = m->getReflectanceColor();
    double p = std::fmax(std::fmax(f.x(), f.y()), f.z());
    if (++dep > maxDep) {
        if (erand48(Xi) < p)
            f *= 1 / p;
        else
            return m->getEmission();
    }
    String type = m->getBsdf();

}
*/

int main(int argc, char *argv[]) {
    int timeStamp = clock();

    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/RT <input scene file> <output bmp file>" << endl;
        return 1;
    }

    Parser parser(argv[1]);
    Camera *camera = parser.getCamera();
    Group* baseGroup = parser.getGroup();

    int w = camera->getWidth(), h = camera->getHeight();
    Image renderedImg(w, h);

    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y) {
            Ray camRay = camera->generateRay(Vector2f(x, y));
            Hit hit;
            bool isIntersect = baseGroup->intersect(camRay, hit, 0);
            if (isIntersect) {
                Vector3f p = camRay.pointAtParameter(hit.getT());
                Vector3f finalColor = Vector3f::ZERO;
                for (int li = 0; li < nLights; ++li) {
                    Light *light = parser.getLight(li);
                    Vector3f L, lightColor;
                    light->getIllumination(p, L, lightColor);
                    finalColor += hit.getMaterial()->Shade(camRay, hit, L, lightColor);
                }
                renderedImg.SetPixel(x, y, finalColor);
            } else renderedImg.SetPixel(x, y, Vector3f::ZERO);
        }
    }
    
    renderedImg.SaveImage(argv[2]);

    cout << "time: " << (clock() - timeStamp) / 1e6 << " sec\n";
    return 0;
}

