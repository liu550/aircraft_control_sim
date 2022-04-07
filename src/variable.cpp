#include <iostream>
#include <math.h>
#include "variable.h"

MathExpressionElement::MathExpressionElement(MathExpressionElementType type, std::string value) : type(type), value(value) {
    if (IsOperator(type) && g_MathOperatorTypeToStringLookupTable[type] != value) {
        std::cout << "Operator type and value " << value << " does not match.\n";
    }
}

// In-order traversal
double MathExpressionElement::Evaluate() {

    double result = 0;

    switch (type) {
        case k_Constant: {
            result = stod(value);
            break;
        }

        // TODO: Variable class should contain a pointer to the model that it belongs to
        case k_Variable: {
            std::map<std::string, Variable*>::iterator it;
            it = variables_lookup_table.find(value);
            if (it == variables_lookup_table.end()) {
                std::cout << "Variable " << value << " cannont be found in the lookup table.\n";
                break;
            } 
            result = it->second->Evaluate();
            break;
        }

        case k_PlusOperator: {
            for (int i = 0; i < children.size(); i++) {
                result += children[i]->Evaluate();
            }
            break;
        }

        case k_MinusOperator: {
            result = children[0]->Evaluate();
            for (int i = 1; i < children.size(); i++) {
                result -= children[i]->Evaluate();
            }
            break;
        }

        case k_MultiplicationOperator:
            result = children[0]->Evaluate();
            for (int i = 1; i < children.size(); i++) {
                result *= children[i]->Evaluate();
            }
            break;

        case k_DivisionOperator:
            result = children[0]->Evaluate();
            for (int i = 1; i < children.size(); i++) {
                result /= children[i]->Evaluate();
            }
            break;

        case k_ExponentiationOperator:
            result = children[0]->Evaluate();
            for (int i = 1; i < children.size(); i++) {
                result = pow(result, children[i]->Evaluate());
            }
            break;
        
        default:
            // Question: Throw exception in default?
            break;
    }

    return result;
}

std::string MathExpressionElement::Print() {

    std::string s = "";

    if (IsOperator(type)) {
        std::string opt = g_MathOperatorTypeToStringLookupTable[type];
        s += "(";
        for (int i = 0; i < children.size() - 1; i++) {
            s += children[i]->Print() + " " + opt + " ";
        }
        s += (children[children.size() - 1])->Print();
        s += ")";
    }

    else if (type == k_Constant) {
        s = value;
    }

    else if (type == k_Variable) {
        std::map<std::string, Variable*>::iterator it;
        it = variables_lookup_table.find(value);
        if (it == variables_lookup_table.end()) {
            std::cout << "Variable " << value << " cannont be found in the lookup table.\n";
        } 
        s = it->second->PrintMathExpression();
    }

    else {
        std::cout << "Cannot print the math expression of the type.\n";
    }

    return s;
}

Variable::Variable(std::string name, MathExpressionElement *expression) : name_(name), expression_(expression) {}