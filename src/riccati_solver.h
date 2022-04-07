#ifndef AIRCRAFT_CONTROL_SIM_RICCATI_SOLVER_H_
#define AIRCRAFT_CONTROL_SIM_RICCATI_SOLVER_H_

#include <eigen3/Eigen/Dense>

using namespace Eigen;

class RiccatiSolver {
public:
    RiccatiSolver(
        MatrixXd A,
        MatrixXd B,
        MatrixXd Q,
        MatrixXd R,
        MatrixXd T,
        MatrixXd E
    );
    ~RiccatiSolver();
    
    MatrixXd Solve();

private:
    MatrixXd A_;
    MatrixXd B_;
    MatrixXd Q_;
    MatrixXd R_;
    MatrixXd T_;
    MatrixXd E_;
};

#endif