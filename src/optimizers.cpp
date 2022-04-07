#include "optimizers.h"
#include "state_model.h"
#include "riccati_solver.h"

void AlgebraicRiccati(LinearStateModel &linear_state_model) {
    
    // Get A and B matrices
    MatrixXd A = linear_state_model.GetA().values;
    MatrixXd B = linear_state_model.GetB().values;

    // Initialize default Q, R, T, and E
    MatrixXd Q(A.cols(), A.cols()); // Dimension of the state vector
    Q.setIdentity();

    MatrixXd R(B.cols(), B.cols()); // Dimension of the control vector
    R.setZero();
    double u_cost = 2;
    for (int i = 0; i < R.rows(); i++) {
        R(i, i) = u_cost;
    }

    MatrixXd T(B.rows(), B.cols());
    T.setZero();

    MatrixXd E(A.rows(), A.cols());
    E.setIdentity();

    RiccatiSolver rs(A, B, Q, R, T, E);
    MatrixXd S = rs.Solve(); // Solution matrix

    MatrixXd K = R.inverse() * B.transpose() * S; // Obtain gain matrix

    linear_state_model.SetK(K);
}
