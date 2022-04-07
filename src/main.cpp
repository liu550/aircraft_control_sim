#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <utility>
#include <string>
#include "dml_parser.h"
#include "model.h"
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigenvalues>
#include <eigen3/Eigen/Eigen>
using namespace Eigen;



std::map<std::string, MathExpressionElementType> g_MathOperatorTypeLookupTable;
std::map<MathExpressionElementType, std::string> g_MathOperatorTypeToStringLookupTable;
int g_SignOperatorCheck = 12;
int g_OperatorCheck = 120;
std::map<std::string, Variable*> variables_lookup_table;

bool IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool IsDigit(char c) {
    return c >= '0' && c <= '9';
}

template<class T>
const T& Max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

template<class T>
void Swap(T& a, T& b) {
    T helper = a;
    a = b;
    b = helper;
}


// Return the index of the dot
// Question: Any problems casting from size_t to int?
int ContainsDot(const std::string &value) {
    std::size_t found = value.find(".");
    if (found == std::string::npos) {
        return -1;
    }
    return found;
}

/*
// TODO: Should be global
bool IsOperator(MathExpressionElementType type) {
    return (type & g_OperatorCheck) == type && type > 0;
}
*/

// TODO: Should be global
bool IsOperator(MathExpressionElementType type) {
    return (type & g_OperatorCheck) == type && type > 0;
}

bool IsSign(MathExpressionElementType type) {
    return (type & g_SignOperatorCheck) == type && type > 0;
}

bool CheckElementFormat(MathExpressionElementType type, std::string sc, const std::string &value, std::string &error) {

    if (sc[0] == ' ') {
        return false; // Ignore white space
    }

    // TODO: Should include '(' as well, but currently does not support implicit multiplication
    // and assume that the format will not be in this way
    if (sc[0] == ')') {
        return false;
    }

    switch (type) {
        case k_Constant: {
            int dot_found = ContainsDot(value);
            if (IsDigit(sc[0]) || (sc[0] == '.' && dot_found < 0)) {
                return true;
            }
            else if (sc[0] == '.' && dot_found >= 0) {
                error = "Format error: A digit cannot have two '.'s\n";
                std::cout << "Format error: A digit cannot have two '.'s\n";
            }
            else if (value[value.size() - 1] == '.') {
                error = "Format error: A digit cannot end with '.'\n";
                std::cout << "Format error: A digit cannot end with '.'\n";
            }
            return false;
        }

        case k_Variable: {
            if (g_MathOperatorTypeLookupTable.find(sc) == g_MathOperatorTypeLookupTable.end()) {
                return true;
            }
            return false;
        }
        
        default: {
            std::cout << "Current type is not supported in ShouldBreak\n";
            return false;
        }
    }
}

// TODO: Currently does not support implict multiplication
bool CheckExpressionFormat(MathExpressionElementType curr_type, MathExpressionElementType prev_type, std::string &error) {

    if (IsOperator(curr_type) && !IsSign(curr_type) && prev_type == k_NullElement) {
        error = "Format error: A non-sign operator cannot be the leading element.\n";
        std::cout << "Format error: A non-sign operator cannot be the leading element.\n";
        return false;
    }

    if (IsOperator(curr_type) && IsOperator(prev_type)) {
        error = "Format error: Two operators cannot be next to each other.\n";
        std::cout << "Format error: Two operators cannot be next to each other.\n";
        return false;
    }

    if (!IsOperator(curr_type) && !IsOperator(prev_type) && prev_type != k_NullElement) {
        error = "Format error: Two operands cannot be next to each other.\n";
        std::cout << "Format error: Two operands cannot be next to each other.\n";
        return false;
    }

    return true;
}


int CompareMathOperator(MathExpressionElement *el1, MathExpressionElement *el2) {
    if  (!IsOperator(el1->type) || !IsOperator(el2->type)) {
        std::cout << "Elements are not operator type\n";
    }

    if (el1->type == el2->type) {
        return 0;
    }

    switch (el1->type) {
        
        case k_PlusOperator:
            if (el2->type == k_MinusOperator) {
                return 1;
            }
            else return -1;

        case k_MinusOperator:
            if (el2->type == k_PlusOperator) {
                return 1;
            }
            else return -1;

        case k_MultiplicationOperator:
            if (el2->type == k_ExponentiationOperator) {
                return -1;
            }
            else return 1;

        case k_DivisionOperator:
            if (el2->type == k_ExponentiationOperator) {
                return -1;
            }
            else return 1;

        case k_ExponentiationOperator:
            return 1;
    }
}

MathExpressionElement* GenerateExpressionTree(std::vector<MathExpressionElement *> &elements, int start, int end, int i) {

    // Corner case: The expression is a single constant or variable
    if (elements.size() == 1) {
        return elements[0];
    }

    // Corner case: Collapse into one subexpression by making all the value elements from start - 1 to end + 1 children of the ith operator
    if (end < i) {
        std::vector<MathExpressionElement *> children;
        for (int j = start - 1; j <= end + 1; j += 2) {
            children.push_back(elements[j]);
        }
        elements[start]->children = children;
        elements[start]->is_subexpression = true;

        // Remove all elements from i - 1 to j - 1 except for i, which is kept as a subexpression
        Swap(elements[start - 1], elements[start]);
        elements.erase(elements.begin() + start, elements.begin() + end + 2);
        return elements[start - 1];
    }

    while (i < elements.size()) {
        //TODO: Check ith and jth elements are indeed operators
        int cmp = CompareMathOperator(elements[start], elements[i]);

        switch (cmp) {
            // The same operator. Advance i
            case 0:
                i += 2;
                break;
            
            // The operator on the left precedes the one on the right (i.e. recursion corner case)
            case 1:
                GenerateExpressionTree(elements, start, i - 2, i);
                start = Max(start - 2, 1); // Move start foreward, since the previous elements have been erased
                i = start + 2;
                end = elements.size() - 2;
                break;

            // The operator on the right precedes the one on the left.
            case -1:
                GenerateExpressionTree(elements, i, end, i);
                i = start + 2;
                end = elements.size() - 2;
                break;
            
            default:
                break;
        }
    }

    return GenerateExpressionTree(elements, start, end, i);
}

// Edge case rules:
// 1. All operators must be followed by either a constant, variable, or a subexpression
// 2. Only '+' and '-' can have no left/preceding constant, variable, or a subexpression
// 3. A value (k_Constant, k_Variable, subexpression) cannot neighbor another value. Neither can an operator neighbor another operator
MathExpressionElement* ParseMathExpression(const std::string &s, int &i, const std::map<std::string, Variable*> &variables_lookup_table) {

    std::vector<MathExpressionElement *> elements;

    std::string value;
    MathExpressionElementType prev_type, curr_type;
    prev_type = k_NullElement;
    curr_type = k_NullElement;

    std::string error;

    while (i < s.size()) {

        char c = s[i];
        std::string sc(1, s[i]); // Stringify the current char to check whether it is an operator


        if (s[i] == ' ') {
            i++;
            continue; // Skip empty space
        }
        
        // Case: k_Variable or k_Constant
        if (IsAlpha(s[i]) || IsDigit(s[i])) {
            prev_type = curr_type;
            curr_type = IsDigit(s[i]) ? k_Constant : k_Variable; // If the leading char is a number, then the string is a constant
            if (!CheckExpressionFormat(curr_type, prev_type, error)) {
                break;
            }

            value += s[i];
            int j =  i + 1;
            while (j < s.size() && CheckElementFormat(curr_type, std::string(1, s[j]), value, error)) {
                i++;
                value += s[j];
                j++;
            }
        }

        // Case: k_Operator
        else if (g_MathOperatorTypeLookupTable.find(sc) != g_MathOperatorTypeLookupTable.end()) { 
            prev_type = curr_type;
            curr_type = g_MathOperatorTypeLookupTable[sc];
            if (!CheckExpressionFormat(curr_type, prev_type, error)) {
                break;
            }

            // Corner case: Sign
            if (curr_type == k_MinusOperator && prev_type == k_NullElement) {
                MathExpressionElement *el = new MathExpressionElement(k_Constant, "-1");
                elements.push_back(el);
                curr_type = k_MultiplicationOperator;
                value = "*";
            }
            else {
                value = sc;
            }
        }

        // Case: Left parenthesis
        else if (s[i] == '(') {

            i++; // Get into the parenthesis
            MathExpressionElement *el = ParseMathExpression(s, i, variables_lookup_table);

            if (el == nullptr) {
                return nullptr;
            }

            curr_type = k_Variable; // A subexpression can be either constant or variable. Does not matter here
            i++; // Get out of the parenthesis
            elements.push_back(el);
            continue;
        }

        // Case: Right parenthesis
        else if (s[i] == ')') {
            GenerateExpressionTree(elements, 1, elements.size() - 2, 1); // Create a tree from the subexpression and return the root
            return elements[0];
        }

        // TODO: Should probably make Variable inherit from MathExpressionElement to make the life easier here
        // Add the pointer to the currently parsed element to the elements vector
        MathExpressionElement *el;
        if (curr_type == k_Variable) {
  
            std::map<std::string, Variable*>::const_iterator kit = variables_lookup_table.find(value);

            if (kit == variables_lookup_table.end()) {
                std::cout << "Variable " << value << " not found.\n";
                return nullptr;
            }
            
            el = kit->second->GetExpression();
        }
        else {
            el = new MathExpressionElement(curr_type, value);
        }
        elements.push_back(el);
        value = "";

        i++;
    }

    GenerateExpressionTree(elements, 1, elements.size() - 2, 1);
    return elements[0];
}

void InitializeMathOperatorLookupTables() {
    g_MathOperatorTypeLookupTable["+"] = k_PlusOperator;
    g_MathOperatorTypeLookupTable["-"] = k_MinusOperator;
    g_MathOperatorTypeLookupTable["*"] = k_MultiplicationOperator;
    g_MathOperatorTypeLookupTable["/"] = k_DivisionOperator;
    g_MathOperatorTypeLookupTable["^"] = k_ExponentiationOperator;

    g_MathOperatorTypeToStringLookupTable[k_PlusOperator] = "+";
    g_MathOperatorTypeToStringLookupTable[k_MinusOperator] = "-";
    g_MathOperatorTypeToStringLookupTable[k_MultiplicationOperator] = "*";
    g_MathOperatorTypeToStringLookupTable[k_DivisionOperator] = "/";
    g_MathOperatorTypeToStringLookupTable[k_ExponentiationOperator] = "^";
}

/*
void InitializeIndependentVariables() {
    std::vector<std::string> x;
    x.push_back("AV_Q");
    x.push_back("AV_S");
    x.push_back("AV_m");
    x.push_back("AV_u_0");

}

void InitializeDependentVariables() {

}
*/

std::vector< std::vector<Variable*> > GenerateLateralDirectionalModeStateVariables() {
    variables_lookup_table["AV_v"] = new Variable("AV_v", nullptr);
    variables_lookup_table["AV_p"] = new Variable("AV_p", nullptr);
    variables_lookup_table["AV_phi"] = new Variable("AV_phi", nullptr);
    variables_lookup_table["AV_r"] = new Variable("AV_r", nullptr);
    std::vector< std::vector<Variable*> > m;
    std::vector<Variable*> m_1;
    m_1.push_back(variables_lookup_table["AV_v"]);
    m_1.push_back(variables_lookup_table["AV_p"]);
    m_1.push_back(variables_lookup_table["AV_phi"]);
    m_1.push_back(variables_lookup_table["AV_r"]);
    m.push_back(m_1);
    return m;
}

std::vector< std::vector<Variable*> > GenerateLateralDirectionalModeControlVariables() {
    variables_lookup_table["AV_delta_r"] = new Variable("AV_delta_r", nullptr);
    variables_lookup_table["AV_delta_a"] = new Variable("AV_delta_a", nullptr);
    std::vector< std::vector<Variable*> > m;
    std::vector<Variable*> m_1;
    m_1.push_back(variables_lookup_table["AV_delta_r"]);
    m_1.push_back(variables_lookup_table["AV_delta_a"]);
    m.push_back(m_1);
    return m;
}

MatrixVariable GenerateInitialConditions() {
    std::vector< std::vector<Variable*> > x_vars = GenerateLateralDirectionalModeStateVariables();
    MatrixXd x_vals(4, 1);

    // Dutch roll
    /*
    x_vals << 97.685,
              -0.1,
               1,
              -0.25;
    */

    // Unit perturbation in sideslip
    x_vals << 279.1,
              0,
              0,
              0;

    MatrixVariable x(x_vars, x_vals);
    return x;
}


MatrixVariable GenerateControlVector() {
    std::vector< std::vector<Variable*> > u_vars = GenerateLateralDirectionalModeControlVariables();
    MatrixXd u_vals(2, 1);
    MatrixVariable u(u_vars, u_vals);
    return u;
}

MatrixVariable GenerateControlMatrix() {
    std::vector< std::vector<Variable*> > B_vars;
    std::vector<Variable*> B_vars_1;
    std::vector<Variable*> B_vars_2;
    std::vector<Variable*> B_vars_3;
    std::vector<Variable*> B_vars_4;

    B_vars_1.push_back(variables_lookup_table["AV_Y_delta_r"]);
    B_vars_1.push_back(variables_lookup_table["AV_Y_delta_a"]);

    B_vars_2.push_back(variables_lookup_table["AV_L_delta_r"]);
    B_vars_2.push_back(variables_lookup_table["AV_L_delta_a"]);

    B_vars_3.push_back(variables_lookup_table["AV_0"]);
    B_vars_3.push_back(variables_lookup_table["AV_0"]);

    B_vars_4.push_back(variables_lookup_table["AV_N_delta_r"]);
    B_vars_4.push_back(variables_lookup_table["AV_N_delta_a"]);

    B_vars.push_back(B_vars_1);
    B_vars.push_back(B_vars_2);
    B_vars.push_back(B_vars_3);
    B_vars.push_back(B_vars_4);

    MatrixVariable B(B_vars);
    B.Evaluate();
    return B;
}

int main() {
    /*
    string fn = "data/Atmospheric_models/F16_package/F16_S119_source/F16_aero.dml";
    DMLParser dml_parser;
    AircraftModel model;

    dml_parser.Parse(fn);
    */


    InitializeMathOperatorLookupTables();

    std::map<std::string, std::string> variables_expression_table;
    std::map<std::string, std::string> variables_expression_table_2;
    std::map<std::string, std::string> *curr_table = &variables_expression_table;
    std::map<std::string, std::string> *next_table = &variables_expression_table_2;
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator end;

    // Dimensional derivatives and aerodynamic coefficients
    variables_expression_table["AV_Q"] = "0.5 * 0.002377 * 279.1^2";
    variables_expression_table["AV_S"] = "5500";
    variables_expression_table["AV_m"] = "17531.838";
    variables_expression_table["AV_u_0"] = "279.1";
    variables_expression_table["AV_c_y_beta"] = "-0.96";
    variables_expression_table["AV_Y_v"] = "AV_Q * AV_S / (AV_m * AV_u_0) * AV_c_y_beta";

    variables_expression_table["AV_b"] = "195.7";
    variables_expression_table["AV_c_y_p"] = "0";
    variables_expression_table["AV_Y_p"] = "AV_Q * AV_S * AV_b / (2 * AV_m * AV_u_0) * AV_c_y_p";

    variables_expression_table["AV_c_y_r"] = "0";
    variables_expression_table["AV_Y_r"] = "AV_Q * AV_S * AV_b / (2 * AV_m * AV_u_0) * AV_c_y_r";

    variables_expression_table["AV_I_x"] = "14.3 * 10^6";
    variables_expression_table["AV_c_l_beta"] = "-0.221";
    variables_expression_table["AV_L_v"] = "AV_Q * AV_S * AV_b / (AV_I_x * AV_u_0) * AV_c_l_beta";

    variables_expression_table["AV_c_l_p"] = "-0.45";
    variables_expression_table["AV_L_p"] = "AV_Q * AV_S * AV_b^2 / (2 * AV_I_x * AV_u_0) * AV_c_l_p";

    variables_expression_table["AV_c_l_r"] = "0.101";
    variables_expression_table["AV_L_r"] = "AV_Q * AV_S * AV_b^2 / (2 * AV_I_x * AV_u_0) * AV_c_l_r";

    variables_expression_table["AV_I_z"] = "45.3 * 10^6";
    variables_expression_table["AV_c_n_beta"] = "0.15";
    variables_expression_table["AV_N_v"] = "AV_Q * AV_S * AV_b / (AV_I_x * AV_u_0) * AV_c_n_beta";

    variables_expression_table["AV_c_n_p"] = "-0.121";
    variables_expression_table["AV_N_p"] = "AV_Q * AV_S * AV_b^2 / (2 * AV_I_z * AV_u_0) * AV_c_n_p";

    variables_expression_table["AV_c_n_r"] = "-0.3";
    variables_expression_table["AV_N_r"] = "AV_Q * AV_S * AV_b^2 / (2 * AV_I_z * AV_u_0) * AV_c_n_r";

    variables_expression_table["AV_Y_delta_r"] = "5.083";
    variables_expression_table["AV_Y_delta_a"] = "0";

    variables_expression_table["AV_L_delta_r"] = "0.0488";
    variables_expression_table["AV_L_delta_a"] = "0.3212";

    variables_expression_table["AV_N_delta_r"] = "-0.2398";
    variables_expression_table["AV_N_delta_a"] = "0.0141";

    // Matrix elements
    variables_expression_table["AV_g_0"] = "32.174";
    variables_expression_table["AV_cos_theta_0"] = "1";
    variables_expression_table["AV_A_13"] = "AV_g_0 * AV_cos_theta_0";
    variables_expression_table["AV_A_14"] = "AV_Y_r - AV_u_0";

    // TODO: Make Variable a derived class of MathExpressionElement
    // TODO: Give actual names to the variables that have the following constant values
    variables_expression_table["AV_0"] = "0";
    variables_expression_table["AV_1"] = "1";

    
    // Q: Why does this not work?
    /*
    variables_expression_table.insert( {"AV_Q", "0.5 * 0.002377 * 279.1^2"} );

    variables_expression_table.insert( std::pair<std::string, std::string>("AV_Q", "0.5 * 0.002377 * 279.1^2") );
    variables_expression_table.insert( std::pair<std::string, std::string>("AV_S", "0.5 * 0.002377 * 279.1^2") );
    variables_expression_table.insert( std::pair<std::string, std::string>("AV_m", "0.5 * 0.002377 * 279.1^2") );
    variables_expression_table.insert( std::pair<std::string, std::string>("AV_u_0", "0.5 * 0.002377 * 279.1^2") );

    variables_expression_table.insert( std::pair<std::string, std::string>("AV_c_y_beta", "0.5 * 0.002377 * 279.1^2") );

    variables_expression_table.insert( std::pair<std::string, std::string>("AV_Y_v", "AV_Q * AV_S / (AV_m * AV_u_0) * AV_c_y_beta") );
    */



    while (variables_expression_table.size() > 0 || variables_expression_table_2.size() > 0) {

        it = variables_expression_table.size() > 0 ? variables_expression_table.begin() : variables_expression_table_2.begin();
        end = variables_expression_table.size() > 0 ? variables_expression_table.end() : variables_expression_table_2.end();
        curr_table = variables_expression_table_2.size() == 0 ? &variables_expression_table : &variables_expression_table_2;
        next_table = variables_expression_table_2.size() == 0 ? &variables_expression_table_2 : &variables_expression_table;

        for (it; it != end; ++it) {
            int i = 0;
            MathExpressionElement *expression = ParseMathExpression(it->second, i, variables_lookup_table);

            if (expression == nullptr) {
                (*next_table)[it->first] = it->second;
                //*next_table[it->first] = it->second;
                continue;
            }

            Variable *variable = new Variable(it->first, expression);
            variables_lookup_table[it->first] = variable;
        }
        (*curr_table).clear();
        //curr_table.clear();
    }



    // Test evaluating Y_v
    /*
    std::map<std::string, Variable*>::iterator test_it;
    test_it = variables_lookup_table.find("AV_Y_v");
    if (test_it == variables_lookup_table.end()) {
        std::cout << "Variable AV_Y_v not found.\n";
    }
    std::cout << "Y_v = " << test_it->second->PrintMathExpression() << "\n";
    std::cout << "Y_v: " << test_it->second->Evaluate() << "\n";
    */
    
    /*
    std::vector< std::vector<Variable*> > A_variables {
        { variables_lookup_table["AV_Y_v"], variables_lookup_table["AV_Y_p"], variables_expression_table["AV_A_13"], variables_expression_table["AV_A_14"] },
        { variables_lookup_table["AV_L_v"], variables_lookup_table["AV_L_p"], variables_expression_table["AV_0"], variables_expression_table["AV_L_r"] },
        { variables_lookup_table["AV_0"], variables_lookup_table["AV_1"], variables_lookup_table["AV_0"], variables_lookup_table["AV_0"] },
        { variables_lookup_table["AV_N_v"], variables_lookup_table["AV_N_p"], variables_expression_table["AV_0"], variables_expression_table["AV_N_r"] }
    };
    */
    //std::vector<Variable*> A_variables_1 = { variables_lookup_table["AV_Y_v"], variables_lookup_table["AV_Y_p"], variables_expression_table["AV_A_13"], variables_expression_table["AV_A_14"] };
    
    std::vector<Variable*> A_variables_1;
    std::vector<Variable*> A_variables_2;
    std::vector<Variable*> A_variables_3;
    std::vector<Variable*> A_variables_4;

    A_variables_1.push_back(variables_lookup_table["AV_Y_v"]);
    A_variables_1.push_back(variables_lookup_table["AV_Y_p"]);
    A_variables_1.push_back(variables_lookup_table["AV_A_13"]);
    A_variables_1.push_back(variables_lookup_table["AV_A_14"]);

    A_variables_2.push_back(variables_lookup_table["AV_L_v"]);
    A_variables_2.push_back(variables_lookup_table["AV_L_p"]);
    A_variables_2.push_back(variables_lookup_table["AV_0"]);
    A_variables_2.push_back(variables_lookup_table["AV_L_r"]);

    A_variables_3.push_back(variables_lookup_table["AV_0"]);
    A_variables_3.push_back(variables_lookup_table["AV_1"]);
    A_variables_3.push_back(variables_lookup_table["AV_0"]);
    A_variables_3.push_back(variables_lookup_table["AV_0"]);

    A_variables_4.push_back(variables_lookup_table["AV_N_v"]);
    A_variables_4.push_back(variables_lookup_table["AV_N_p"]);
    A_variables_4.push_back(variables_lookup_table["AV_0"]);
    A_variables_4.push_back(variables_lookup_table["AV_N_r"]);
    
    std::vector< std::vector<Variable*> > A_variables;
    A_variables.push_back(A_variables_1);
    A_variables.push_back(A_variables_2);
    A_variables.push_back(A_variables_3);
    A_variables.push_back(A_variables_4);

    // TODO: Should evaluate A inside state model
    MatrixVariable A(A_variables);
    A.Evaluate();

    // Test A
    /*
    std::cout << "A:\n" << A.values << "\n";
    

    std::cout << "Eigenvalues:\n" << A.values.eigenvalues() << "\n";
    EigenSolver<MatrixXd> es(A.values);
    std::cout << "Eigenvalus (EigenSolver):\n" << es.eigenvalues() << "\n";
    std::cout << "Eigenvectors:\n" << es.eigenvectors() << "\n";
    */


    // Initialize lateral/directional state variables:
    // Sideslip, row rate, row angle, yaw rate
    /*
    variables_lookup_table["AV_v"] = new Variable("AV_v", nullptr);
    variables_lookup_table["AV_p"] = new Variable("AV_p", nullptr);
    variables_lookup_table["AV_phi"] = new Variable("AV_phi", nullptr);
    variables_lookup_table["AV_r"] = new Variable("AV_r", nullptr);
    std::vector< std::vector<Variable*> > lateral_directional_state_variables;
    std::vector<Variable*> lateral_directional_state_variables_1;
    lateral_directional_state_variables_1.push_back(variables_lookup_table["AV_v"]);
    lateral_directional_state_variables_1.push_back(variables_lookup_table["AV_p"]);
    lateral_directional_state_variables_1.push_back(variables_lookup_table["AV_phi"]);
    lateral_directional_state_variables_1.push_back(variables_lookup_table["AV_r"]);
    lateral_directional_state_variables.push_back(lateral_directional_state_variables_1);

    MatrixXd dutch_roll_initial_conditions(4, 1);
    dutch_roll_initial_conditions << 97.685,
                                     -0.1,
                                     1,
                                     -0.25;
    x = MatrixVariable(lateral_directional_state_variables, dutch_roll_initial_conditions);

    std::vector< std::vector<Variable*> > x_d_vars;
    MatrixXd x_d_vals(x.values.rows(), x.values.cols());
    x_d_vals.setZero();
    x_d = MatrixVariable(x_d_vars, x_d_vals);

    std::vector< std::vector<Variable*> > B_vars;
    MatrixXd B_vals(x.values.rows(), 2);
    B_vals.setZero();
    B = MatrixVariable(B_vars, B_vals);

    std::vector< std::vector<Variable*> > u_vars;
    MatrixXd u_vals(2, 1);
    u_vals.setZero();
    u = MatrixVariable(u_vars, u_vals);

    std::vector< std::vector<Variable*> > B_vars;
    MatrixXd B_vals(x.values.rows(), u.values.rows());
    B_vals.setZero();
    B = MatrixVariable(B_vars, B_vals);
    */

    MatrixVariable x, x_d, u, B;

    x = GenerateInitialConditions();
    u = GenerateControlVector();
    B = GenerateControlMatrix();

    std::vector< std::vector<Variable*> > x_d_vars;
    MatrixXd x_d_vals(x.values.rows(), x.values.cols());
    x_d_vals.setZero();
    x_d = MatrixVariable(x_d_vars, x_d_vals);

    LinearStateModel linear_state_model = LinearStateModel(x, x_d, u, A, B); // Initialize linear state model

    // Trim and run the model
    linear_state_model.Trim();
    linear_state_model.Update(1e-4, 20, true);
}