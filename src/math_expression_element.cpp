#include <iostream>
#include "math_expression_element.h"
#include <math.h>
/*
MathExpressionElement::MathExpressionElement(MathExpressionElementType type, std::string value) : type(type), value(value) {}

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
*/