#ifndef AIRCRAFT_CONTROL_SIM_MATHML_ELEMENT_H_
#define AIRCRAFT_CONTROL_SIM_MATHML_ELEMENT_H_

#include <string>
#include <vector>
using namespace std;

enum MathMLElementType {
    MathMLConstant,
    MathMLVariable,
    MathMLOperator
};

struct MathMLElement {
    vector<MathMLElement *> children;
    MathMLElementType type;
    string val;

    double Evaluate();
};

#endif