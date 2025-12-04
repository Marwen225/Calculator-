#ifndef EXPRESSION_H
#define EXPRESSION_H

typedef enum {
    OPERATOR,
    CONSTANT,
    VARIABLE
} TokenType;

typedef struct TreeNode {
    TokenType type;
    union {
        char operator;
        double constant;
        char variable;
    } value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

#endif // EXPRESSION_H
