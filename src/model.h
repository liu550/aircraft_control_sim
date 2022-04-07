#ifndef AIRCRAFT_CONTROL_SIM_MODEL_H_
#define AIRCRAFT_CONTROL_SIM_MODEL_H_

#include "state_model.h"
#include "variable.h"
#include <string>

/*
enum class AircraftVariable {
    // Configuration
    AircraftVariable_m,
    AircraftVariable_S,

    // State
    AircraftVariable_u,
    AircraftVariable_v,
    AircraftVariable_w,
    AircraftVariable_p,
    AircraftVariable_q,
    AircraftVariable_r,
    AircraftVariable_l,
    AircraftVariable_m,
    AircraftVariable_n,

    // Dimensional lateral/directional aerodynamic derivatives
    AircraftVariable_y_v,
    AircraftVariable_y_p,
    AircraftVariable_y_r,
    AircraftVariable_l_v,
    AircraftVariable_l_p,
    AircraftVariable_l_r,
    AircraftVariable_n_v,
    AircraftVariable_n_p,
    AircraftVariable_n_r,
    
    // Dimentionless lateral/directional aerodynamic coefficients
    AircraftVariable_c_y_beta,
    AircraftVariable_c_y_p,
    AircraftVariable_c_y_r,
    AircraftVariable_c_l_beta,
    AircraftVariable_c_l_p,
    AircraftVariable_c_l_r,
    AircraftVariable_c_n_beta,
    AircraftVariable_c_n_p,
    AircraftVariable_c_n_r,
};

std::map<string, AircraftVariable> g_AircraftVariableNameLookupTable = {
    { "AircraftVariable_m,", AircraftVariable_m },
    { "AircraftVariable_S",  AircraftVariable_S }

};
*/

class Model {
public:
    Model(std::map<std::string, Variable*> variables);

    void SetStateModel();

    

private:
    std::map<std::string, Variable*> variables_;
    StateModel state_model_;
};

#endif