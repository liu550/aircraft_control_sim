// Declares and defines all the integration methods

#include "integrations.h"
#include "state_model.h"

void RK4(StateModel &state_model, double dt) {

    MatrixXd x_0, x_temp, x_1;
    MatrixXd k_1, k_2, k_3, k_4;

    // k1
    x_0 = state_model.GetStateValues();
    k_1 = state_model.EvaluateDerivatives();

    // k2
    x_temp = x_0 + dt / 2 * k_1;
    state_model.SetStateValues(x_temp);
    k_2 = state_model.EvaluateDerivatives();

    // k3
    x_temp = x_0 + dt / 2 * k_2;
    state_model.SetStateValues(x_temp);
    k_3 = state_model.EvaluateDerivatives();

    // k4
    x_temp = x_0 + dt * k_3;
    state_model.SetStateValues(x_temp);
    k_4 = state_model.EvaluateDerivatives();

    // Update state
    MatrixXd d = (double)1 / (double)6 * dt * (k_1 + k_2 + k_3 + k_4);
    x_1 = x_0 + d;
    state_model.SetStateValues(x_1);

}