#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree_builder.h"
#include "stack.h"
#include "utils.h"

TreeNode* createNode(TokenType type) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node) {
        node->type = type;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

TreeNode* buildExpressionTree(char* postfix) {
    if (!postfix) return NULL;

    // Estimate capacity for stack (length of string is safe upper bound)
    Stack* stack = createStack(strlen(postfix));
    if (!stack) return NULL;

    char* token = strtok(postfix, " ");
    while (token != NULL) {
        if (isdigit(token[0])) {
            TreeNode* node = createNode(CONSTANT);
            node->value.constant = atof(token);
            push(stack, node);
        } else if (isalpha(token[0])) {
            TreeNode* node = createNode(VARIABLE);
            node->value.variable = token[0];
            push(stack, node);
        } else if (isOperator(token[0])) {
            TreeNode* node = createNode(OPERATOR);
            node->value.operator = token[0];
            
            node->right = pop(stack);
            node->left = pop(stack);
            
            push(stack, node);
        }
        token = strtok(NULL, " ");
    }

    TreeNode* root = pop(stack);
    // Stack should be empty now if expression was valid
    // We could check isEmpty(stack) but for simplicity we assume valid postfix from our converter
    
    // Free the stack structure (but not the nodes!)
    // Our createStack mallocs the struct and the array.
    // We need a freeStack function or just free manually.
    // Since we didn't implement freeStack in stack.h, let's just free manually here or add it.
    // For now, manual free to avoid changing stack.h again unless necessary.
    if (stack) {
        free(stack->array);
        free(stack);
    }

    return root;
}
