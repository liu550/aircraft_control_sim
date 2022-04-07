#ifndef AIRCRAFT_CONTROL_SIM_MATH_EXPRESSION_ELEMENT_H_
#define AIRCRAFT_CONTROL_SIM_MATH_EXPRESSION_ELEMENT_H_

#include <map>
#include <string>
#include <vector>

/*
std::map<std::string, MathExpressionElementType> g_MathExpressionElementTypeLookupTable = {
    { "+", k_PlusOperator },
    { "-", k_MinusOperator },
    { "*", k_MultiplicationOperator },
    { "/", k_DivisionOperator },
    { "^", k_ExponentiationOperator }
};
*/

/*
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
};

extern std::map<std::string, MathExpressionElementType> g_MathOperatorTypeLookupTable;
extern int g_OperatorCheck;

extern std::map<std::string, Variable*> variables_lookup_table;
*/
#endif