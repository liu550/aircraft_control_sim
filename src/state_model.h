#ifndef AIRCRAFT_CONTROL_SIM_STATE_MODEL_H_
#define AIRCRAFT_CONTROL_SIM_STATE_MODEL_H_

#include <iostream>
#include "variable.h"
#include "integrations.h"
#include "optimizers.h"
#include <eigen3/Eigen/Dense>


using namespace Eigen;


struct MatrixVariable {
    std::vector< std::vector<Variable*> > variables;
    MatrixXd values;

    MatrixVariable();
    MatrixVariable(std::vector< std::vector<Variable*> > vars);
    MatrixVariable(std::vector< std::vector<Variable*> > vars, MatrixXd vals);

    void Evaluate();

    void Print(std::string &s);
};

class StateModel {
public:
    StateModel();
    StateModel(MatrixVariable x, MatrixVariable x_d, MatrixVariable u);
    // TODO: Implement the default method
    // Default state model
    // Assumes that models of x_d are already defined
    virtual MatrixXd EvaluateDerivatives() {
        MatrixXd x_d;
        return x_d;
    };

    virtual Optimizer GetOptimizer() {
        return k_NelderMeadSimplex;
    }

    virtual void Trim() {
        std::cout << "Does not offer default trimming method yet\n";
    }

    virtual void UpdateControl() {
        std::cout << "Does not offer any control update on default\n";
    }

    // Compute state evolution
    void Update(double dt, double duration, bool should_print);

    const MatrixXd& GetStateValues();

    const MatrixXd& GetStateDerivativeValues();

    void SetStateValues(MatrixXd values);

    void SetStateDerivativeValues(const MatrixXd &values);

protected:
    MatrixVariable x_;
    MatrixVariable x_d_;
    MatrixVariable u_;

    // Default integration method is RK4
    IntegrationMethod integration_method_ = k_RK4;

    double t_ = 0;
};

class LinearStateModel : public StateModel {
public:

    LinearStateModel(MatrixVariable x, MatrixVariable x_d, MatrixVariable u, MatrixVariable A, MatrixVariable B);
    // Linear state model
    // x_d = Ax + Bu
    MatrixXd EvaluateDerivatives();

    Optimizer GetOptimizer();

    void Trim();

    void UpdateControl();

    MatrixVariable GetA();

    MatrixVariable GetB();

    void SetK(MatrixXd K);

private:
    MatrixVariable A_;
    MatrixVariable B_;
    MatrixXd K_;
};

#endif