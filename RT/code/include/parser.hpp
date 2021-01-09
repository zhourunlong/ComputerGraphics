#pragma once

#include <bits/stdc++.h>
#include "pugi/pugixml.hpp"
#include "bsdf/bsdf.h"
#include "camera.hpp"
#include "texture/bump.hpp"
#include "texture/texture.h"
#include "object/object3d.hpp"
#include "object/group.hpp"
#include "object/mesh.hpp"
#include "object/sphere.hpp"
#include "object/triangle.hpp"
#include "object/transform.hpp"
#include "object/curve.hpp"
#include "object/revsurface.hpp"
#include "vecmath/vecmath.h"
#include "image/ClmbsImg.hpp"
#include "emitter.hpp"

double degreeToRadian(const double &x) {return (M_PI * x) / 180.0;}

double stringToDouble(const std::string &s, const int &st, int &ed) {
    int len = s.length(), rst;
    for (rst = st; rst < len && (s[rst] == ' ' || s[rst] == ','); ++rst);
    for (ed = rst; ed < len && s[ed] != ' ' && s[ed] != ','; ++ed);
    return strtod(s.substr(rst, ed - rst).c_str(), 0);
}

int countSpaces(const std::string &s) {
    int cnt = 0;
    for (int i = 1; i < s.size(); ++i)
        if (s[i] == ' ' && s[i - 1] != ' ')
            ++cnt;
    if (cnt && s[s.size() - 1] == ' ')
        --cnt;
    return cnt;
}

Vector3d stringToV3d(const std::string &s) {
    double a, b, c;
    int p, q;
    a = stringToDouble(s, 0, p);
    b = stringToDouble(s, p, q);
    c = stringToDouble(s, q, p);
    return Vector3d(a, b, c);
}

bool endsWith(const std::string &s, const std::string &t) {
    if (s.length() < t.length()) return false;
    std::string r = s.substr(s.length() - t.length(), s.length());
    return r == t;
}

std::string deformat(const std::string &s) {
    std::string t = "";
    for (int i = 0; i < s.length(); ++i) {
        if ('a' <= s[i] && s[i] <= 'z') t += s[i];
        if ('A' <= s[i] && s[i] <= 'Z') t += char(s[i] - 'A' + 'a');
    }
    return t;
}

std::pair<std::string, Vector3d> parseV3d(const pugi::xml_attribute &att) {
    std::string val1 = att.value();
    assert(att.name() == "name");
    pugi::xml_attribute attr = att.next_attribute();
    assert(attr.name() == "value");
    return make_pair(deformat(val1), stringToV3d(attr.value()));
}

std::pair<std::string, double> parseDouble(const pugi::xml_attribute &att) {
    std::string val1 = att.value();
    assert(att.name() == "name");
    pugi::xml_attribute attr = att.next_attribute();
    assert(attr.name() == "value");
    return make_pair(deformat(val1), attr.as_double());
}

std::pair<std::string, std::string> parseString(const pugi::xml_attribute &att) {
    std::string val1 = att.value();
    assert(att.name() == "name");
    pugi::xml_attribute attr = att.next_attribute();
    assert(attr.name() == "value");
    return make_pair(deformat(val1), attr.value());
}

std::pair<std::string, int> parseInt(const pugi::xml_attribute &att) {
    std::string val1 = att.value();
    assert(att.name() == "name");
    pugi::xml_attribute attr = att.next_attribute();
    assert(attr.name() == "value");
    return make_pair(deformat(val1), attr.as_int());
}

class Parser {
public:

    Parser(const char* fname) {
        fprintf(stderr, "started parsing\n");
        Material* m = new Diffuse();
        m->setType(Material::DIFFUSE);
        m->setDiffRefl(tex05);
        materialMap[""] = m;
        pugi::xml_document doc;
        if (!doc.load_file(fname)) {
            printf("Error loading scene file!\n");
            exit(-1);
        }
        //dfs(doc, 0);
        lights.clear();
        parse(doc);
        concludeAllObjects();
        checkLights();
        group->finish();
        fprintf(stderr, "finished parsing\n");
    }

    ~Parser() = default;

    Camera *getCamera() const {return camera;}

    Group *getGroup() const {return group;}

    int getMaxDep() const {return maxDep;}

    int getSampleCount() const {return samps;}

    double getGamma() const {return gamma;}

    std::vector <Object3D*> getLights() {return lights;}

    std::vector <Emitter*> getEmitters() {return emitters;}

    void dfs(const pugi::xml_node &node, int dep);

private:

    std::string genNextId() {
        while (1) {
            std::string id = "#" + std::to_string(idCounter++) + "#zrl2018011309";
            if (materialMap.find(id) == materialMap.end())
                return id;
        }
    }

    void parseSensor(const pugi::xml_node &node);
    void parseTexture(const pugi::xml_node &node, Texture* &tex);
    void parseBsdf(const pugi::xml_node &node, Material* &m, int dep, int pass, bool bump, Bump* &obump);
    void parseTransform(const pugi::xml_node &node, Transform* &tran);
    void parseSphere(const pugi::xml_node &node, Sphere* &sph);
    void parseTriangle(const pugi::xml_node &node, Triangle* &tri, const bool &isRoot);
    void parseMesh(const pugi::xml_node &node, Mesh* &m);
    void parseCurve(const pugi::xml_node &node, Curve* &cur);
    void parseRevSurface(const pugi::xml_node &node, RevSurface* &rev);
    void parseShape(const pugi::xml_node &node);
    void parseEmitter(const pugi::xml_node &node, Emitter* &emitter);
    void parse(const pugi::xml_node &node);
    void concludeAllObjects() {
        int n = group->getGroupSize();
        for (int i = 0; i < n; ++i) {
            Object3D* obj = group->getObj(i);
            obj->setMaterial(materialMap[obj->getMatRef()]);
        }
    }
    void checkLights() {
        int n = group->getGroupSize();
        for (int i = 0; i < n; ++i) {
            Object3D* obj = group->getObj(i);
            if (obj->isSampleable()) lights.push_back(obj);
        }
    }
    
    int idCounter = 0;
    int maxDep = 5;
    int samps = 100;
    double gamma = 2.2;
    Camera *camera = new Camera();
    std::unordered_map <std::string, Material*> materialMap;
    std::unordered_map <std::string, Mesh*> meshMap;
    std::unordered_map <std::string, ClmbsImg_Data*> imageMap;
    Group *group = new Group();
    std::vector <Object3D*> lights;
    std::vector <Emitter*> emitters;
    Texture* tex05 = new Texture(Vector3d(0.5));
    Texture* tex1 = new Texture(Vector3d(1));
};

void Parser::parseSensor(const pugi::xml_node &node) {
    std::string nname = node.name();
    if (nname == "double" || nname == "float") {
        std::pair<std::string, double> result = parseDouble(node.first_attribute());
        if (result.first == "fov")
            camera->setAngle(degreeToRadian(result.second));
        else if (result.first == "gamma")
            gamma = result.second;
        else if (result.first == "focusdistance")
            camera->setDistance(result.second);
        else if (result.first == "apertureradius")
            camera->setAperture(result.second);
        return;
    }
    if (nname == "transform") {
        Transform* tran = new Transform();
        parseTransform(node, tran);
        Matrix4d M = tran->getTransform();
        Vector4d center(0, 0, 0, 1), target(0, 0, 1, 1), up(0, 1, 0, 1); 
        center = M * center;
        target = M * target;
        up = M * up;
        camera->setCenter(center.xyz());
        camera->setTarget(target.xyz());
        camera->setUp(up.xyz());
        delete tran;
        return;
    }
    if (nname == "integer") {
        std::pair<std::string, int> result = parseInt(node.first_attribute());
        if (result.first == "width")
            camera->setWidth(result.second);
        else if (result.first == "height")
            camera->setHeight(result.second);
        else if (result.first == "samplecount")
            samps = result.second;
        return;
    }
    if (nname == "string") {
        std::pair<std::string, std::string> result = parseString(node.first_attribute());
        if (result.first == "fovaxis")
            camera->setFovAxis(result.second);
        return;
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseSensor(child);
}

void Parser::parseTexture(const pugi::xml_node &node, Texture* &tex) {
    std::string nname = node.name();
    std::pair<std::string, std::string> result = parseString(node.first_attribute());
    if (result.first == "filename") {
        ClmbsImg_Data* img;
        if (imageMap.find(result.second) != imageMap.end())
            img = imageMap[result.second];
        else {
            img = new ClmbsImg_Data(
                ClmbsImg_Load(result.second.c_str()));
            imageMap[result.second] = img;
        }
        tex->setBitmap(img);
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseTexture(child, tex);
}

void Parser::parseBsdf(const pugi::xml_node &node, Material* &m, int dep, int pass, bool bump, Bump* &obump) {
    std::string nname = node.name(),
                aname = node.first_attribute().name(),
                val = node.first_attribute().value();
    if (nname == "bsdf" && aname == "type" && val == "bumpmap")
        bump = true;
  if (pass == 1) {
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "eta")
            m->setEta(result.second);
        else if (result.first == "k")
            m->setK(result.second);
        Texture* tex = new Texture(result.second);
        if (bump && obump) {
            Bump* nbump = new Bump(obump);
            nbump->setNestedBitmap(tex);
            if (result.first == "diffusereflectance"
                || result.first == "reflectance")
                
                m->setDiffRefl(nbump);
            else if (result.first == "specularreflectance")
                m->setSpecRefl(nbump);
            else if (endsWith(result.first, "transmittance"))
                m->setTran(nbump);
        } else {
            if (result.first == "diffusereflectance"
                || result.first == "reflectance")
                
                m->setDiffRefl(tex);
            else if (result.first == "specularreflectance")
                m->setSpecRefl(tex);
            else if (endsWith(result.first, "transmittance"))
                m->setTran(tex);
        }
        return;
    }
    if (nname == "double" || nname == "float") {
        std::pair<std::string, double> result = parseDouble(node.first_attribute());
        if (result.first == "intior")
            m->setIntIor(result.second);
        else if (result.first == "extior")
            m->setExtIor(result.second);
        else if (result.first == "alpha")
            m->setAlpha(result.second);
        else if (result.first == "exteta")
            m->setExtEta(result.second);
        return;
    }
    if (nname == "string") {
        std::pair<std::string, std::string> result = parseString(node.first_attribute());
        if (result.first == "material" && deformat(result.second) != "none") {
            std::cout << "[X] material name is not supported for now\n";
            std::cout << "[X] check https://pixelandpoly.com/ior.html and https://github.com/tunabrain/tungsten/blob/master/src/core/bsdfs/ComplexIorData.hpp\n";
        }
    }
    
    if (nname == "texture") {
        std::pair<std::string, std::string> result = parseString(node.first_attribute());
        if (result.second == "bitmap") {
            if (result.first == "map") {
                obump = new Bump();
                Texture* tex = static_cast<Texture*>(obump);
                parseTexture(node, tex);
            } else {
                Texture* tex = new Texture();
                parseTexture(node, tex);
                if (bump && obump) {
                    Bump* nbump = new Bump(obump);
                    nbump->setNestedBitmap(tex);
                    if (result.first == "diffusereflectance"
                        || result.first == "reflectance")
                        
                        m->setDiffRefl(nbump);
                    else if (result.first == "specularreflectance")
                        m->setSpecRefl(nbump);
                    else if (endsWith(result.first, "transmittance"))
                        m->setTran(nbump);
                } else {
                    if (result.first == "diffusereflectance"
                        || result.first == "reflectance")
                        
                        m->setDiffRefl(tex);
                    else if (result.first == "specularreflectance")
                        m->setSpecRefl(tex);
                    else if (endsWith(result.first, "transmittance"))
                        m->setTran(tex);
                }
            }
        }
    }
  }
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        std::string aname = attr.name(), val = attr.value();
        if (nname == "bsdf") {
            if (aname == "type") {
                if (pass == 1) {
                    if (val == "twosided") m->setTwoSided(true);
                } else if (pass == 0) {
                    if (val == "diffuse" || val == "roughdiffuse") {
                        m = new Diffuse();
                        m->setType(Material::DIFFUSE);
                        m->setDiffRefl(tex05);
                    } else if (val == "conductor") {
                        m = new Conductor();
                        m->setType(Material::CONDUCTOR);
                        m->setSpecRefl(tex1);
                        m->setExtEta(1);
                        m->setEta(Vector3d(0));
                        m->setK(Vector3d(1));
                    } else if (val == "roughconductor") {
                        m = new RoughConductor();
                        m->setType(Material::ROUGHCONDUCTOR);
                        m->setSpecRefl(tex1);
                        m->setAlpha(0.1);
                        m->setExtEta(1);
                    } else if (val == "dielectric" || val == "roughdielectric") {
                        m = new Dielectric();
                        m->setType(Material::DIELECTRIC);
                        m->setSpecRefl(tex1);
                        m->setTran(tex1);
                        m->setTwoSided(true);
                        m->setIntIor(1.5046);
                        m->setExtIor(1.000277);
                    } else if (val == "thindielectric") {
                        m = new ThinDielectric();
                        m->setType(Material::THINDIELECTRIC);
                        m->setSpecRefl(tex1);
                        m->setTran(tex1);
                        m->setTwoSided(true);
                        m->setIntIor(1.5046);
                        m->setExtIor(1.000277);
                    } else if (val == "plastic") {
                        m = new Plastic();
                        m->setType(Material::PLASTIC);
                        m->setDiffRefl(tex05);
                        m->setSpecRefl(tex1);
                        m->setIntIor(1.49);
                        m->setExtIor(1.000277);
                    } else if (val == "roughplastic") {
                        m = new RoughPlastic();
                        m->setType(Material::ROUGHPLASTIC);
                        m->setDiffRefl(tex05);
                        m->setSpecRefl(tex1);
                        m->setIntIor(1.49);
                        m->setExtIor(1.000277);
                        m->setAlpha(0.1);
                    }
                }
            } else if (pass == 1 && aname == "id")
                m->setId(val);
        }
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseBsdf(child, m, dep + 1, pass, bump, obump);
    if (dep == 0 && pass == 1) {
        if (m->getId() == "")
            m->setId(genNextId());
        materialMap[m->getId()] = m;
    }
}

void Parser::parseTransform(const pugi::xml_node &node, Transform* &tran) {
    std::string nname = node.name();
    if (nname == "scale") {
        std::string tmp = node.first_attribute().value();
        if (tmp.find(',') != -1) {
            Vector3d s = stringToV3d(tmp);
            tran->appendTransform(Matrix4d::scaling(s[0], s[1], s[2]));
        } else {
            double s = node.first_attribute().as_double();
            tran->appendTransform(Matrix4d::uniformScaling(s));
        }
    } else if (nname == "translate") {
        Vector3d result = stringToV3d(node.first_attribute().value());
        tran->appendTransform(Matrix4d::translation(result));
    } else if (nname == "rotate") {
        pugi::xml_attribute attr = node.first_attribute();
        Vector3d axis = stringToV3d(attr.value());
        tran->appendTransform(Matrix4d::rotation(axis, degreeToRadian(attr.next_attribute().as_double())));
    } else if (nname == "matrix") {
        Matrix4d mat = Matrix4d::identity();
        std::string s = node.first_attribute().value();
        int bg = 0, ed;
        if (countSpaces(s) > 10) {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j) {
                    double v = stringToDouble(s, bg, ed);
                    bg = ed;
                    mat(i, j) = v;
                }
        } else {
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j) {
                    double v = stringToDouble(s, bg, ed);
                    bg = ed;
                    mat(i, j) = v;
                }
        }
        tran->appendTransform(mat);
    } else if (nname == "lookat") {
        Vector3d center, target, up, horizon;
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            std::string aname = attr.name();
            Vector3d result = stringToV3d(attr.value());
            if (aname == "origin") center = result;
            else if (aname == "target") target = result;
            else if (aname == "up") up = result;
        }
        target -= center;
        horizon = Vector3d::cross(target, up);
        Matrix4d M(-horizon.x(), up.x(), target.x(), center.x(),
                   -horizon.y(), up.y(), target.y(), center.y(),
                   -horizon.z(), up.z(), target.z(), center.z(),
                   0,            0,      0,          1);
        tran->appendTransform(M);
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseTransform(child, tran);
}

void Parser::parseSphere(const pugi::xml_node &node, Sphere* &sph) {
    bool isRoot = false;
    if (sph == NULL) {
        isRoot = true;
        sph = new Sphere();
    }
    std::string nname = node.name();
    if (nname == "double" || nname == "float") {
        std::pair<std::string, double> result = parseDouble(node.first_attribute());
        if (result.first == "radius")
            sph->setRadius(result.second);
        return;
    }
    if (nname == "translate") {
        sph->setCenter(stringToV3d(node.first_attribute().value()));
        return;
    }
    if (nname == "ref") {
        sph->setMatRef(node.first_attribute().value());
        return;
    }
    if (nname == "bsdf") {
        Material *m = NULL;
        Bump* obump = NULL;
        parseBsdf(node, m, 0, 0, false, obump);
        parseBsdf(node, m, 0, 1, false, obump);
        sph->setMatRef(m->getId());
        return;
    }
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "radiance")
            sph->setEmission(result.second);
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseSphere(child, sph);
    if (isRoot)
        group->addObject(sph);
}

void Parser::parseTriangle(const pugi::xml_node &node, Triangle* &tri, const bool &isRoot = false) {
    std::string nname = node.name();
    if (nname == "transform") {
        tri->setNeedTransform(true);
        Transform* tran = new Transform();
        parseTransform(node, tran);
        tri->setTransform(tran);
        tran->setObject(tri);
        group->addObject(tran);
        return;
    }
    if (nname == "ref") {
        if (tri->getNeedTransform())
            tri->getTransform()->setMatRef(node.first_attribute().value());
        else
            tri->setMatRef(node.first_attribute().value());
        return;
    }
    if (nname == "bsdf") {
        Material *m = NULL;
        Bump* obump = NULL;
        parseBsdf(node, m, 0, 0, false, obump);
        parseBsdf(node, m, 0, 1, false, obump);
        if (tri->getNeedTransform())
            tri->getTransform()->setMatRef(m->getId());
        else
            tri->setMatRef(m->getId());
        return;
    }
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "radiance")
            if (tri->getNeedTransform())
                tri->getTransform()->setEmission(result.second);
            else
                tri->setEmission(result.second);
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseTriangle(child, tri);
    if (isRoot && tri->getNeedTransform() == false)
        group->addObject(tri);
}

void Parser::parseMesh(const pugi::xml_node &node, Mesh* &mesh) {
    bool isRoot = false;
    if (mesh == NULL) {
        isRoot = true;
        mesh = new Mesh();
    }
    std::string nname = node.name();
    if (nname == "string") {
        std::pair<std::string, std::string> result = parseString(node.first_attribute());
        if (result.first == "filename") {
            if (meshMap.find(result.second) == meshMap.end()) {
                mesh->setFile(result.second.c_str());
                meshMap[result.second] = mesh;
            } else mesh = meshMap[result.second];
        }
        return;
    }
    if (nname == "transform") {
        mesh->setNeedTransform(true);
        Transform* tran = new Transform();
        parseTransform(node, tran);
        mesh->setTransform(tran);
        tran->setObject(mesh);
        group->addObject(tran);
        return;
    }
    if (nname == "ref") {
        if (mesh->getNeedTransform())
            mesh->getTransform()->setMatRef(node.first_attribute().value());
        else
            mesh->setMatRef(node.first_attribute().value());
        return;
    }
    if (nname == "bsdf") {
        Material *m = NULL;
        Bump* obump = NULL;
        parseBsdf(node, m, 0, 0, false, obump);
        parseBsdf(node, m, 0, 1, false, obump);
        if (mesh->getNeedTransform())
            mesh->getTransform()->setMatRef(m->getId());
        else
            mesh->setMatRef(m->getId());
        return;
    }
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "radiance")
            if (mesh->getNeedTransform())
                mesh->getTransform()->setEmission(result.second);
            else
                mesh->setEmission(result.second);
    }
    if (nname == "boolean") {
        std::pair<std::string, std::string> result = parseString(node.first_attribute());
        if (result.first == "facenormals")
            if (deformat(result.second) == "true") mesh->setFaceNorm(true);
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseMesh(child, mesh);
    if (isRoot && mesh->getNeedTransform() == false)
        group->addObject(mesh);
}

void Parser::parseCurve(const pugi::xml_node &node, Curve* &cur) {
    std::vector <Vector3d> controls;
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
        std::string tmp = child.first_attribute().as_string();
        controls.push_back(stringToV3d(tmp));
    }
    std::string type = deformat(node.first_attribute().as_string());
    if (type == "bezier") cur = new BezierCurve(controls);
    else if (type == "bspline") cur = new BsplineCurve(controls);
}

void Parser::parseRevSurface(const pugi::xml_node &node, RevSurface* &rev) {
    bool isRoot = false;
    if (rev == NULL) {
        isRoot = true;
        rev = new RevSurface();
    }
    std::string nname = node.name();
    if (nname == "transform") {
        rev->setNeedTransform(true);
        Transform* tran = new Transform();
        parseTransform(node, tran);
        rev->setTransform(tran);
        tran->setObject(rev);
        group->addObject(tran);
        return;
    }
    if (nname == "ref") {
        if (rev->getNeedTransform())
            rev->getTransform()->setMatRef(node.first_attribute().value());
        else
            rev->setMatRef(node.first_attribute().value());
        return;
    }
    if (nname == "bsdf") {
        Material *m = NULL;
        Bump* obump = NULL;
        parseBsdf(node, m, 0, 0, false, obump);
        parseBsdf(node, m, 0, 1, false, obump);
        if (rev->getNeedTransform())
            rev->getTransform()->setMatRef(m->getId());
        else
            rev->setMatRef(m->getId());
        return;
    }
    if (nname == "curve") {
        Curve* cur = NULL;
        parseCurve(node, cur);
        rev->setCurve(cur);
        return;
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseRevSurface(child, rev);
    if (isRoot && rev->getNeedTransform() == false)
        group->addObject(rev);
}

void Parser::parseShape(const pugi::xml_node &node) {
    pugi::xml_attribute attr = node.first_attribute();
    std::string type = attr.value();
    if (type == "sphere") {
        Sphere *m = NULL;
        parseSphere(node, m);
        return;
    } else if (type == "rectangle") {
        Triangle *m1 = new Triangle(Vector3d(1, 1, 0), Vector3d(-1, -1, 0), Vector3d(1, -1, 0)),
                 *m2 = new Triangle(Vector3d(1, 1, 0), Vector3d(-1, 1, 0), Vector3d(-1, -1, 0));
        parseTriangle(node, m1, true);
        parseTriangle(node, m2, true);
        return;
    } else if (type == "obj") {
        Mesh *mesh = NULL;
        parseMesh(node, mesh);
        return;
    } else if (type == "revsurface") {
        RevSurface *rev = NULL;
        parseRevSurface(node, rev);
        return;
    }
}

void Parser::parseEmitter(const pugi::xml_node &node, Emitter* &emitter) {
    std::string nname = node.name();
    if (nname == "emitter") {
        pugi::xml_attribute attr = node.first_attribute();
        std::string type = attr.value();
        if (type == "constant") emitter = new ConstEmitter();
    }
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "radiance")
            emitter->setRadiance(result.second);
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseEmitter(child, emitter);
}

void Parser::parse(const pugi::xml_node &node) {
    std::string nname = node.name();
    if (nname == "integrator") {
        std::pair<std::string, int> result = parseInt(node.first_child().first_attribute());
        if (result.first == "maxdepth")
            maxDep = result.second;
        return;
    }
    if (nname == "sensor") {
        parseSensor(node);
        camera->finish();
        return;
    }
    if (nname == "shape") {
        parseShape(node);
        return;
    }
    if (nname == "bsdf") {
        Material *m = NULL;
        Bump* obump = NULL;
        parseBsdf(node, m, 0, 0, false, obump);
        parseBsdf(node, m, 0, 1, false, obump);
        return;
    }
    if (nname == "emitter") {
        Emitter* emitter = NULL;
        parseEmitter(node, emitter);
        emitters.push_back(emitter);
        return;
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parse(child);
}

void Parser::dfs(const pugi::xml_node &node, int dep) {
    for (int i = 0; i < dep; ++i) std::cout << "  ";
    std::string nname = node.name();
    std::cout << "name=" << nname << ", attr={";
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        std::string aname = attr.name(), val = attr.value();
        std::cout << "(" << aname << ", " << val << ") ";
    }
    std::cout << "}\n";
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        dfs(child, dep + 1);
}
