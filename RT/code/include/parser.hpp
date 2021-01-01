#pragma once

#include <bits/stdc++.h>
#include "pugi/pugixml.hpp"
#include "material.hpp"
#include "camera.hpp"
#include "object3d.hpp"
#include "group.hpp"
#include "mesh.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "transform.hpp"
#include "vecmath/vecmath.h"

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
        Material* m = new Material();
        materialMap[""] = m;
        pugi::xml_document doc;
        if (!doc.load_file(fname)) {
            printf("Error loading scene file!\n");
            exit(-1);
        }
        //dfs(doc, 0);
        lights.clear();
        parse(doc);
        setMatToAll();
        checkLights();
        group->finish();
    }

    ~Parser() = default;

    Camera *getCamera() const {return camera;}

    Group *getGroup() const {return group;}

    int getMaxDep() const {return maxDep;}

    int getSampleCount() const {return samps;}

    double getGamma() const {return gamma;}

    std::vector <Object3D*> getLights() {return lights;}

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
    void parseBsdf(const pugi::xml_node &node, Material* &m);
    void parseTransform(const pugi::xml_node &node, Transform* &tran);
    void parseSphere(const pugi::xml_node &node, Sphere* &sph);
    void parseTriangle(const pugi::xml_node &node, Triangle* &tri, const bool &isRoot);
    void parseMesh(const pugi::xml_node &node, Mesh* &m);
    void parseShape(const pugi::xml_node &node);
    void parse(const pugi::xml_node &node);
    void setMatToAll() {
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
            if (obj->getObjType() == Object3D::TRANSFORM) {
                Transform* _obj = static_cast<Transform*>(obj);
                assert(_obj->getObject()->getObjType() == Object3D::TRIANGLE);
                Triangle* tri = static_cast<Triangle*>(_obj->getObject());
                if (tri->getEmmision() != Vector3d::ZERO)
                    lights.push_back(obj);
            } else {
                if (obj->getEmmision() != Vector3d::ZERO) {
                    assert(obj->getObjType() == Object3D::TRIANGLE);
                    lights.push_back(obj);
                }
            }
        }
    }
    
    int idCounter = 0;
    int maxDep = 5;
    int samps = 100;
    double gamma = 2.2;
    Camera *camera = new Camera();
    std::map <std::string, Material*> materialMap;
    Group *group = new Group();
    std::vector <Object3D*> lights;
};

void Parser::parseSensor(const pugi::xml_node &node) {
    std::string nname = node.name();
    if (nname == "double" || nname == "float") {
        std::pair<std::string, double> result = parseDouble(node.first_attribute());
        if (result.first == "fov")
            camera->setAngle(degreeToRadian(result.second));
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
    if (nname == "float") {
        std::pair<std::string, double> result = parseDouble(node.first_attribute());
        if (result.first == "gamma")
            gamma = result.second;
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseSensor(child);
}

void Parser::parseBsdf(const pugi::xml_node &node, Material* &m) {
    bool isRoot = false;
    if (m == NULL) {
        isRoot = true;
        m = new Material();
    }
    std::string nname = node.name();
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (endsWith(result.first, "reflectance"))
            m->setRefl(result.second);
        else if (endsWith(result.first, "transmittance"))
            m->setTran(result.second);
        return;
    }
    if (nname == "double" || nname == "float") {
        std::pair<std::string, double> result = parseDouble(node.first_attribute());
        if (result.first == "intior")
            m->setIntIor(result.second);
        else if (result.first == "extior")
            m->setExtIor(result.second);
        return;
    }
    if (nname == "string") {
        std::pair<std::string, std::string> result = parseString(node.first_attribute());
        if (result.first == "material")
            m->setSurfMaterial(result.second);
        return;
    }
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        std::string aname = attr.name(), val = attr.value();
        if (nname == "bsdf") {
            if (aname == "type") {
                if (val == "twosided") m->setTwoSided(true);
                else if (val == "diffuse") m->setType(Material::DIFF);
                else if (val == "conductor") m->setType(Material::COND);
                else if (val == "dielectric") {
                    m->setType(Material::DIEL);
                    m->setTwoSided(true);
                }
            } else if (aname == "id")
                m->setId(val);
        }
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseBsdf(child, m);
    if (isRoot) {
        if (m->getId() == "")
            m->setId(genNextId());
        materialMap[m->getId()] = m;
    }
}

void Parser::parseTransform(const pugi::xml_node &node, Transform* &tran) {
    std::string nname = node.name();
    if (nname == "scale") {
        std::string tmp = node.first_attribute().next_attribute().value();
        if (tmp.find(',') != -1) {
            std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
            Vector3d s = result.second;
            tran->appendTransform(Matrix4d::scaling(s[0], s[1], s[2]));
        } else {
            std::pair<std::string, double> result = parseDouble(node.first_attribute());
            tran->appendTransform(Matrix4d::uniformScaling(result.second));
        }
    } else if (nname == "translate") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        tran->appendTransform(Matrix4d::translation(result.second));
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
        parseBsdf(node, m);
        sph->setMatRef(m->getId());
        return;
    }
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "radiance")
            sph->setEmmision(result.second);
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
        tran->setObject(tri);
        group->addObject(tran);
        return;
    }
    if (nname == "ref") {
        tri->setMatRef(node.first_attribute().value());
        return;
    }
    if (nname == "bsdf") {
        Material *m = NULL;
        parseBsdf(node, m);
        tri->setMatRef(m->getId());
        return;
    }
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "radiance")
            tri->setEmmision(result.second);
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
        if (result.first == "filename")
            mesh->setFile(result.second.c_str());
        return;
    }
    if (nname == "transform") {
        mesh->setNeedTransform(true);
        Transform* tran = new Transform();
        parseTransform(node, tran);
        tran->setObject(mesh);
        group->addObject(tran);
        return;
    }
    if (nname == "ref") {
        mesh->setMatRef(node.first_attribute().value());
        return;
    }
    if (nname == "bsdf") {
        Material *m = NULL;
        parseBsdf(node, m);
        mesh->setMatRef(m->getId());
        return;
    }
    if (nname == "rgb") {
        std::pair<std::string, Vector3d> result = parseV3d(node.first_attribute());
        if (result.first == "radiance")
            mesh->setEmmision(result.second);
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseMesh(child, mesh);
    if (isRoot && mesh->getNeedTransform() == false)
        group->addObject(mesh);
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
    }
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
        parseBsdf(node, m);
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
