#include <iostream>
#include "riccati_solver.h"

void QRDecomposition(const MatrixXd &A, MatrixXd &Q, MatrixXd &R) {
    auto qr = A.householderQr();
    Q = qr.householderQ();
    R = qr.matrixQR();
    //R = qr.matrixQR().triangularView<Upper>();
}

void QRIteration(MatrixXd &A, MatrixXd &U, double tolerance) {
    
    MatrixXd A0, A_prev, Q, R, Q_prev, R_prev;
    A0 = A;
    A_prev = A;
    double max_abs_diff = 1;
    int num_of_iter = 1;

    QRDecomposition(A_prev, Q_prev, R_prev);
    U = Q_prev;

    std::cout << "A0:\n" << A_prev << std::endl
            << "Q0:\n" << Q_prev << std::endl
            << "R0:\n" << R_prev << std::endl;

    while (num_of_iter < 10000) {
        A = R_prev * Q_prev;
        max_abs_diff = (A - A_prev).cwiseAbs().maxCoeff();
        
        A_prev = A;
        QRDecomposition(A, Q, R);
        Q_prev = Q;
        U *= Q;
        R_prev = R;
        num_of_iter++;
    }

    /*
    MatrixXd U_21 = U.bottomLeftCorner(U.rows() / 2, U.cols() / 2);
    MatrixXd U_11 = U.topLeftCorner(U.rows() / 2, U.cols() / 2);
    MatrixXd X = U_21 * U_11.inverse();

    std::cout << "num_of_iter:\n" << num_of_iter << std::endl
        << "max_abs_diff:\n" << max_abs_diff << std::endl
        << "A:\n" << A << std::endl
        << "U:\n" << U << std::endl
    */
}

RiccatiSolver::RiccatiSolver(
    MatrixXd A,
    MatrixXd B,
    MatrixXd Q,
    MatrixXd R,
    MatrixXd T,
    MatrixXd E)
    : 
    A_(A),
    B_(B),
    Q_(Q),
    R_(R),
    T_(T),
    E_(E) 
    {
    }

RiccatiSolver::~RiccatiSolver() {

}

// Schur method
MatrixXd RiccatiSolver::Solve() {
    // Get the Hamiltonian matrix Z
    // Z << A, -B*Inverse(R)*Transpose(B)
    //     -Q, -Transpose(A)

    // 1st row of Z
    MatrixXd G = B_ * R_.inverse() * B_.transpose();
    MatrixXd m1(A_.rows(), A_.cols() + G.cols());
    m1 << A_, -G;

    // 2nd row of Z
    MatrixXd m2(Q_.rows(), Q_.cols() + A_.rows());
    m2 << -Q_, -A_.transpose();

    // Concatenate the two rows
    MatrixXd Z(m1.rows() + m2.rows(), m1.cols());
    Z << m1,
         m2;
    
    // Test the correctness of Z
    /*
    std::cout << "m1:\n" << m1 << std::endl
              << "m2:\n" << m2 << std::endl
              << "Z:\n" << Z << std::endl;
    */

    // Reduce Z to its Schur form using QR algorithm.
    
    MatrixXd U;
    double tolerance = 1e-6;
    QRIteration(Z, U, tolerance);
    

    // Test QR decomposition
    /*
    MatrixXd A(3, 3);
    A << 12, -51, 4,
         6, 167, -68,
        -4, 24, -41;
    QRIteration(A, U, tolerance);
    */

    // During the process, obtain the transformation matrix U.
    MatrixXd U_21 = U.bottomLeftCorner(U.rows() / 2, U.cols() / 2);
    MatrixXd U_11 = U.topLeftCorner(U.rows() / 2, U.cols() / 2);
    MatrixXd S = U_21 * U_11.inverse();

    return S;
}





/*
int main() {
    MatrixXd m1(2, 2);
    m1 << 1, 2,
          4, 5;
    cout << m1 << endl;
    cout << "Inverse: " << m1.inverse() << endl;
    cout << "Determinant: " << m1.determinant() << endl;
    return 0;
}
*/
