#include "pugixml.hpp"
#include <iostream>

const char* node_types[] = {
    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};

void dfs(pugi::xml_node node, int dep) {
    for (int i = 0; i < dep; ++i) std::cout << "  ";
    std::cout << node_types[node.type()] << ": name=" << node.name() << ", attr={";
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        std::cout << "(" << attr.name() << ", " << attr.value() << ") ";
    }
    std::cout << "}\n";
    for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
        dfs(child, dep + 1);
}

int main() {
    pugi::xml_document doc;
    if (!doc.load_file("cbox2.xml")) return -1;
    dfs(doc, 0);
}
