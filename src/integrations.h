#ifndef AIRCRAFT_CONTROL_SIM_INTEGRATIONS_H_
#define AIRCRAFT_CONTROL_SIM_INTEGRATIONS_H_

#include <map>
#include <eigen3/Eigen/Dense>

using namespace Eigen;

class StateModel;

enum IntegrationMethod {
    k_RK4
};

void RK4(StateModel &state_model, double dt);

/*
std::map<IntegrationMethod, void *> g_IntegrationMethodLookupTable = {
   { k_RK4, &RK4 }
};
*/
#endif