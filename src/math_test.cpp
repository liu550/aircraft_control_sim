// math_test.cpp tests solver programs

#include "riccati_solver.h"

void TestRiccatiSolver() {
    MatrixXd A(2, 2);
    MatrixXd B(2, 1);
    MatrixXd R(1, 1);
    MatrixXd Q(2, 2);
    MatrixXd T(B.rows(), B.cols());
    MatrixXd E(A.rows(), A.cols());

    A << 0, 1,
         0, 0;

    B << 0,
         1;
    
    R.setIdentity();

    Q << 1, 0,
         0, 2;

    T.setZero();

    E.setIdentity();

    RiccatiSolver rs(A, B, Q, R, T, E);

    MatrixXd result = rs.Solve();
}

int main() {
    TestRiccatiSolver();
}