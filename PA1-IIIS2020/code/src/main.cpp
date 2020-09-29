#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    int timeStamp = clock();

    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }

    SceneParser sceneParser(argv[1]);
    Camera *camera = sceneParser.getCamera();
    Group* baseGroup = sceneParser.getGroup();
    Vector3f bgColor = sceneParser.getBackgroundColor();

    int w = camera->getWidth(), h = camera->getHeight(),
        nLights = sceneParser.getNumLights();
    Image renderedImg(w, h);

    for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y) {
            Ray camRay = camera->generateRay(Vector2f(x, y));
            Hit hit;
            bool isIntersect = baseGroup->intersect(camRay, hit, 0);
            if (isIntersect) {
                Vector3f p = camRay.pointAtParameter(hit.getT());
                Vector3f finalColor = Vector3f::ZERO;
                for (int li = 0; li < nLights; ++li) {
                    Light *light = sceneParser.getLight(li);
                    Vector3f L, lightColor;
                    light->getIllumination(p, L, lightColor);
                    finalColor += hit.getMaterial()->Shade(camRay, hit, L, lightColor);
                }
                renderedImg.SetPixel(x, y, finalColor);
            } else renderedImg.SetPixel(x, y, bgColor);
        }
    
    renderedImg.SaveImage(argv[2]);
    
    cout << "Hello! Computer Graphics!" << endl;
    cout << "time: " << (clock() - timeStamp) / 1e6 << " sec\n";
    return 0;
}

