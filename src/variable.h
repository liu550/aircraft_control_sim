#ifndef AIRCRAFT_CONTROL_SIM_VARIABLE_H_
#define AIRCRAFT_CONTROL_SIM_VARIABLE_H_

#include <map>
#include <string>
#include <vector>

enum MathExpressionElementType {
    k_NullElement = 0,
    k_Constant = 1,
    k_Variable = 2,
    k_PlusOperator = 4,
    k_MinusOperator = 8,
    k_MultiplicationOperator = 16,
    k_DivisionOperator = 32,
    k_ExponentiationOperator = 64
};

struct MathExpressionElement {
    std::vector<MathExpressionElement *> children;
    MathExpressionElementType type;
    std::string value;
    bool is_subexpression;

    MathExpressionElement(MathExpressionElementType type, std::string value);

    double Evaluate();

    std::string Print();
};

class Variable {
public:
    
    Variable(std::string name, MathExpressionElement *expression);

    // Default way to calculate the value of the variable is through a predefined equation
    virtual double Evaluate() {
        return expression_->Evaluate();
    }

    // TODO: Should make it return a const pointer instead
    MathExpressionElement* GetExpression() {
        return expression_;
    }

    std::string PrintMathExpression() {
        return expression_->Print();
    }


private:
    std::string name_;
    std::string id_;
    std::string units_;
    std::string sign_;
    std::string description_;

    MathExpressionElement *expression_;
};

// Variable that is solved for through using a lookup table and interpolation
struct LookupTable {
    std::vector<std::string> independent_variables;
    std::vector< std::vector<double> > breakpoints;
    std::vector< std::vector<double> > vals;
};

struct ApproximatedFunctionWithLookupTable : Variable {
    LookupTable lookup_table;

    double Evaluate() {
        return 0;
    }
};

bool IsOperator(MathExpressionElementType type);

extern std::map<std::string, MathExpressionElementType> g_MathOperatorTypeLookupTable;
extern std::map<MathExpressionElementType, std::string> g_MathOperatorTypeToStringLookupTable;
extern int g_SignOperatorCheck;
extern int g_OperatorCheck;

// TODO: Variables_lookup_table should not be global. Instead, as shown in math_expression_element.cpp,
// each Variable should maintain a pointer to the model it belongs to and thus the corresponding table
extern std::map<std::string, Variable*> variables_lookup_table;

#endif