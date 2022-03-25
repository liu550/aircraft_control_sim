#ifndef AIRCRAFT_CONTROL_SIM_MODEL_H_
#define AIRCRAFT_CONTROL_SIM_MODEL_H_

#include "variable.h"
#include <map>
using namespace std;

struct AircraftModel {
    map<string, Variable> variables; 
};

#endif