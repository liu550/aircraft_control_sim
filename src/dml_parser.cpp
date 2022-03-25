#include "dml_parser.h"
#include <iostream>
using namespace std;

void DMLParser::Parse(string fn) {
    
    xmlDoc *doc = xmlReadFile(fn.c_str(), nullptr, 0);

    if (doc == nullptr) {
        cout << "ERROR: Failed to create a XML document.\n";
        return;
    }

    xmlNode *root = xmlDocGetRootElement(doc);

    if (root == nullptr) {
        cout << "ERROR: Root element is null.\n";
        return;
    }

    cout << "Name: " << root->name << "\n";
    cout << "Attribute: " << root->properties->name << "\n";
    cout << "Attribute: " << root->properties->next->name << "\n";
    cout << "Child: " << root->children->name << "\n";
    cout << "Child: " << root->children->next->name << "\n";
}
