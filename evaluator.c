#include <stdio.h>
#include <stdlib.h>
#include "evaluator.h"

double evaluateTree(TreeNode* root, char varName, double varValue) {
    if (!root) return 0.0;

    if (root->type == CONSTANT) {
        return root->value.constant;
    }

    if (root->type == VARIABLE) {
        if (root->value.variable == varName) {
            return varValue;
        } else {
            // Variable not found or not supported in this simple version
            // For now, return 0 or maybe print a warning?
            // User spec: "vérifie si son nom correspond à `varName` et retourne `varValue` si c'est le cas."
            // Doesn't say what to do if not. Let's assume 0 for simplicity or print error.
            printf("Error: Unknown variable '%c'\n", root->value.variable);
            return 0.0;
        }
    }

    if (root->type == OPERATOR) {
        double leftVal = evaluateTree(root->left, varName, varValue);
        double rightVal = evaluateTree(root->right, varName, varValue);

        switch (root->value.operator) {
            case '+': return leftVal + rightVal;
            case '-': return leftVal - rightVal;
            case '*': return leftVal * rightVal;
            case '/':
                if (rightVal == 0.0) {
                    printf("Error: Division by zero\n");
                    return 0.0;
                }
                return leftVal / rightVal;
            default:
                printf("Error: Unknown operator '%c'\n", root->value.operator);
                return 0.0;
        }
    }

    return 0.0;
}
