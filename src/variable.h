#ifndef AIRCRAFT_CONTROL_SIM_VARIABLE_H_
#define AIRCRAFT_CONTROL_SIM_VARIABLE_H_

#include "mathml_element.h"
using namespace std;

struct Variable {
    string name;
    string id;
    string units;
    string sign;
    string description;

    virtual double Evaluate() {
        return 0;
    }
};

// Variable that has an exact function to be solved
struct Function : Variable {
    MathMLElement *expression;

    double Evaluate() {
        return expression->Evaluate();
    }
};

// Variable that is solved for through using a lookup table and interpolation
struct LookupTable {
    vector<string> independent_variables;
    vector< vector<double> > breakpoints;
    vector< vector<double> > vals;
};

struct ApproximatedFunctionWithLookupTable : Variable {
    LookupTable lookup_table;

    double Evaluate() {
        return 0;
    }
};

#endif