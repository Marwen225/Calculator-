#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expression.h"
#include "converter.h"
#include "tree_builder.h"
#include "simplifier.h"
#include "evaluator.h"

// Print tree in inorder notation
void printInorder(TreeNode* node) {
    if (!node) return;
    
    if (node->type == CONSTANT) {
        printf("%.1f", node->value.constant);
    } else if (node->type == VARIABLE) {
        printf("%c", node->value.variable);
    } else if (node->type == OPERATOR) {
        printf("(");
        printInorder(node->left);
        printf(" %c ", node->value.operator);
        printInorder(node->right);
        printf(")");
    }
}

// Free all nodes in the tree
void freeTree(TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

int main(int argc, char *argv[]) {
    // Default values
    char infix[256] = "(x + 0) * (5 - 3)";
    double x_value = 10.0;
    
    // Parse command-line arguments
    if (argc >= 2) {
        strncpy(infix, argv[1], sizeof(infix) - 1);
        infix[sizeof(infix) - 1] = '\0';
    }
    if (argc >= 3) {
        x_value = atof(argv[2]);
    }
    
    printf("=== Expression Calculator ===\n\n");
    printf("Original infix expression: %s\n\n", infix);
    
    // 2. Convert to postfix
    char* postfix = infixToPostfix(infix);
    if (!postfix) {
        printf("Error: Failed to convert to postfix\n");
        return 1;
    }
    printf("Postfix expression: %s\n\n", postfix);
    
    // 3. Build expression tree
    // Need to make a copy since buildExpressionTree modifies the string
    char postfixCopy[256];
    strncpy(postfixCopy, postfix, sizeof(postfixCopy));
    postfixCopy[sizeof(postfixCopy) - 1] = '\0';
    
    TreeNode* root = buildExpressionTree(postfixCopy);
    if (!root) {
        printf("Error: Failed to build expression tree\n");
        free(postfix);
        return 1;
    }
    
    // 4. Print original tree
    printf("Original tree (inorder): ");
    printInorder(root);
    printf("\n\n");
    
    // 5. Simplify tree
    simplifyTree(&root);
    
    // 6. Print simplified tree
    printf("Simplified tree (inorder): ");
    printInorder(root);
    printf("\n\n");
    
    // 7. Evaluate tree with x value
    printf("Evaluating with x = %.1f:\n", x_value);
    double result = evaluateTree(root, 'x', x_value);
    printf("Result: %.2f\n\n", result);
    
    // 8. Free memory
    freeTree(root);
    free(postfix);
    
    printf("=== Calculation Complete ===\n");
    return 0;
}
