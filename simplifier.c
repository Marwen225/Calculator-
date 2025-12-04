#include <stdio.h>
#include <stdlib.h>
#include "simplifier.h"

// Helper to free a node and its children recursively
static void freeTree(TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

void simplifyTree(TreeNode** nodeRef) {
    if (!nodeRef || !*nodeRef) return;

    TreeNode* node = *nodeRef;

    // Recursively simplify children first (post-order)
    simplifyTree(&(node->left));
    simplifyTree(&(node->right));

    // If leaf node, nothing to do
    if (node->type != OPERATOR) return;

    TreeNode* left = node->left;
    TreeNode* right = node->right;

    // 1. Constant Folding
    if (left && right && left->type == CONSTANT && right->type == CONSTANT) {
        double result = 0.0;
        switch (node->value.operator) {
            case '+': result = left->value.constant + right->value.constant; break;
            case '-': result = left->value.constant - right->value.constant; break;
            case '*': result = left->value.constant * right->value.constant; break;
            case '/': 
                if (right->value.constant != 0)
                    result = left->value.constant / right->value.constant; 
                else {
                    // Division by zero, maybe keep as is or handle error?
                    // For now, let's just not simplify if div by zero
                    return; 
                }
                break;
            default: return;
        }

        // Create new constant node
        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->type = CONSTANT;
        newNode->value.constant = result;
        newNode->left = NULL;
        newNode->right = NULL;

        // Free old subtree
        freeTree(node);

        // Update pointer
        *nodeRef = newNode;
        return;
    }

    // 2. Identity '+' (x + 0 = x, 0 + x = x)
    if (node->value.operator == '+') {
        if (right && right->type == CONSTANT && right->value.constant == 0.0) {
            // Replace node with left child
            *nodeRef = left;
            // Detach left from node so it's not freed
            node->left = NULL; 
            freeTree(node);
            return;
        }
        if (left && left->type == CONSTANT && left->value.constant == 0.0) {
            // Replace node with right child
            *nodeRef = right;
            node->right = NULL;
            freeTree(node);
            return;
        }
    }

    // 3. Identity '*' (x * 1 = x, 1 * x = x)
    if (node->value.operator == '*') {
        if (right && right->type == CONSTANT && right->value.constant == 1.0) {
            *nodeRef = left;
            node->left = NULL;
            freeTree(node);
            return;
        }
        if (left && left->type == CONSTANT && left->value.constant == 1.0) {
            *nodeRef = right;
            node->right = NULL;
            freeTree(node);
            return;
        }
    }

    // 4. Zero Property '*' (x * 0 = 0, 0 * x = 0)
    if (node->value.operator == '*') {
        if ((right && right->type == CONSTANT && right->value.constant == 0.0) ||
            (left && left->type == CONSTANT && left->value.constant == 0.0)) {
            
            // Create new 0 constant node
            TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
            newNode->type = CONSTANT;
            newNode->value.constant = 0.0;
            newNode->left = NULL;
            newNode->right = NULL;

            freeTree(node);
            *nodeRef = newNode;
            return;
        }
    }
}
