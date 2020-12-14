#include <bits/stdc++.h>
#include "pugixml.hpp"
#include "material.hpp"
#include <vecmath.h>

int idCounter = 0;
std::map <std::string, Material> materialMap;

inline float stringToFloat(const std::string &s, const int &st, int &ed) {
    int len = s.length(), rst;
    for (rst = st; rst < len && (s[rst] == ' ' || s[rst] == ','); ++rst);
    for (ed = rst; ed < len && s[ed] != ' ' && s[ed] != ','; ++ed);
    return strtod(s.substr(rst, ed - rst).c_str(), 0);
}

inline Vector3f stringToV3f(const std::string &s) {
    float a, b, c;
    int p, q;
    a = stringToFloat(s, 0, p);
    b = stringToFloat(s, p, q);
    c = stringToFloat(s, q, q);
    return Vector3f(a, b, c);
}

inline std::pair<std::string, Vector3f> parseRgb(const pugi::xmxl_node &node) {
    pugi::xml_attribute attr = node.first_attribute();
    std::string aname1 = attr.name(), val1 = attr.value();
    assert(aname1 == "name");
    attr = attr.next_attribute();
    std::string aname2 = attr.name(), val2 = attr.value();
    assert(aname2 == "value");
    return make_pair(val1, stringToV3f(val2));
}

std::string genNextId() {
    while (1) {
        std::string id = "#" + to_string(idCounter++) + "zrl2018011309";
        if (materialMap.find(id) == materialMap.end())
            return id;
    }
}

void parseBdsf(pugi::xml_node node, Material *&m) {
    bool isRoot = false;
    if (m == NULL) {
        isRoot = true;
        m = new Material();
    }
    std::string nname = node.name();
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        std::string aname = attr.name(), val = attr.value();
        if (nname == "bsdf") {
            switch (aname) {
                case "type": {
                    switch (val) {
                        case "twosided": {m->setTwoSided(true); break;}
                        case "diffuse": {m->setType(DIFF); break;}
                        case "conductor": {m->setType(COND); break;}
                        case "dielectric": {m->setType(DIEL); break;}
                    }
                    break;
                }
                case "id": {m->setId(val); break;}
                break;
            }
        } else {

        }
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parseBsdf(child, m);
    if (isRoot) {
        if (m.getId() == "")
            m.setId(genNextId());
        materialMap[m.getId()] = m;
        delete(m);
    }
}

void parse(pugi::xml_node node) {
    std::string nname = node.name();
    if (nname == "integrator") {

    }
    if (nname == "sensor") {

    }
    if (nname == "shape") {
        
    }
    if (nname == "bsdf") {
        parseBsdf(node, NULL);
        return;
    }
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        parse(child);
}

void dfs(pugi::xml_node node, int dep, void* ret) {
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

int main(int argc, char* argv[]) {
    pugi::xml_document doc;
    if (!doc.load_file(argv[1])) return -1;
    dfs(doc, 0);
    parse(doc);
}
