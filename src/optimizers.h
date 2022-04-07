#ifndef AIRCRAFT_CONTROL_SIM_OPTIMIZERS_H_
#define AIRCRAFT_CONTROL_SIM_OPTIMIZERS_H_

#include <eigen3/Eigen/Dense>

using namespace Eigen;

class StateModel;
class LinearStateModel;

enum Optimizer {
    k_AlgebraicRiccati,
    k_NelderMeadSimplex
};

void AlgebraicRiccati(LinearStateModel &linear_state_model);

#endif