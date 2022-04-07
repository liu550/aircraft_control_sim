#include "state_model.h"
#include <stdio.h>

MatrixVariable::MatrixVariable() {}

MatrixVariable::MatrixVariable(std::vector< std::vector<Variable*> > vars) : variables(vars) {} 

MatrixVariable::MatrixVariable(std::vector< std::vector<Variable*> > vars, MatrixXd vals) : variables(vars), values(vals) {}

void MatrixVariable::Print(std::string &s) {
    for (int i = 0; i < values.rows(); i++) {
        for (int j = 0; j < values.cols(); j++) {
            s += std::to_string(values(i, j)) + " ";
            std::cout << values(i, j) << " ";
        }
    }
}

void MatrixVariable::Evaluate() {

    MatrixXd m(variables.size(), variables[0].size());

    for (int i = 0; i < variables.size(); i++) {
        std::vector<Variable*> &row = variables[i];
        for (int j = 0; j < row.size(); j++) {
            auto val = row[j]->Evaluate();
            m(i, j) = val;
        }
    }

    values = m;
}

StateModel::StateModel() {}

// TODO: Check dimensions
StateModel::StateModel(MatrixVariable x, MatrixVariable x_d, MatrixVariable u) :
x_(x), x_d_(x_d), u_(u) {} 

// Default state model
void StateModel::Update(double dt, double duration, bool should_print_results) {

    double t_0 = t_;
    FILE *fp; 
    std::string result = "";

    while (t_ <= duration) {
        if (should_print_results && (t_ == 0 || t_ - t_0 >= 0.01)) {
            result += std::to_string(t_) + " ";
            std::cout << "t=" << t_ << " ";

            // Print state vector
            x_.Print(result);
            /*
            for (int i = 0; i < x_.values.rows(); i++) {
                for (int j = 0; j < x_.values.cols(); j++) {
                    result += std::to_string(x_.values(i, j)) + " ";
                    std::cout << x_.values(i, j) << " ";
                }
            }
            */
            UpdateControl(); // Update control vector through the linear relationship: u = -K*u2
            u_.Print(result); 

            t_0 = t_;
            result += "\n";
            std::cout << "\n";
        }

        switch (integration_method_) {
            case k_RK4:
                RK4(*this, dt);
                break;
            
            default:
                std::cout << "Integration method not found\n";
                return;
        }   

        t_ += dt;
    }

    // Write to file
    fp = fopen("lat-dir-trim-c_u=2.txt", "wb");
    char buffer[result.size() + 1];
    std::strcpy(buffer, result.c_str());
    fwrite(buffer, sizeof(char), sizeof(buffer), fp);
    fclose(fp);


    /*
    std::map<IntegrationMethod, void (*)(StateModel&)>::iterator it;

    it = g_IntegrationMethodLookupTable.find(integration_method_);
    if (it == g_IntegrationMethodLookupTable.end()) {
        std::cout << "Integration method requested is not found.\n";
        return; 
    }

    void (*f)(StateModel&) = it->second;

    f(*this);
    */
}

const MatrixXd& StateModel::GetStateValues() {
    return x_.values;
}

const MatrixXd& StateModel::GetStateDerivativeValues() {
    return x_d_.values;
}

void StateModel::SetStateValues(MatrixXd values) {
    // TODO: Check dimension
    x_.values = values;
}

void StateModel::SetStateDerivativeValues(const MatrixXd &values) {
    // TODO: Check dimension
    x_d_.values = values;
};

// Linear state model
// TODO: Check dimensions
LinearStateModel::LinearStateModel(MatrixVariable x, MatrixVariable x_d, MatrixVariable u, MatrixVariable A, MatrixVariable B) :
StateModel(x, x_d, u), A_(A), B_(B) {
    // Initialize K to zero matrix on default
    MatrixXd initial_K(u_.values.rows(), x_.values.rows());
    initial_K.setZero();
    SetK(initial_K);
}

MatrixXd LinearStateModel::EvaluateDerivatives() {
    // Since the model is linear, there is no need to update A, B, and K matrices during integration

    try {
        MatrixXd result = A_.values * x_.values + B_.values * u_.values;
        return result;
    }
    catch (std::exception &e) {
        std::cout << "An exception occured when evaluating linear state model derivatives: " << e.what();
    }
}

Optimizer LinearStateModel::GetOptimizer() {
    return k_AlgebraicRiccati;
}

void LinearStateModel::Trim() {
    
    Optimizer opt = GetOptimizer();

    switch (opt) {
        case k_AlgebraicRiccati:
            // TODO: Cast to linear state model
            AlgebraicRiccati(*this);
            break;
        
        default:
            std::cout << "The selected optimizer is not supported\n";
            break;
    }
}

void LinearStateModel::UpdateControl() {
    u_.values = -K_ * x_.values;
}

MatrixVariable LinearStateModel::GetA() {
    return A_;
}

MatrixVariable LinearStateModel::GetB() {
    return B_;
}

void LinearStateModel::SetK(MatrixXd K) {
    if (K.rows() != u_.values.rows() || K.cols() != x_.values.rows()) {
        std::cout << "Control gain matrix dimension does not match the one(s) of the state and control vectors\n";
        return;
    }
    K_ = K;
}
