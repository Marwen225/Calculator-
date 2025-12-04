#ifndef STACK_H
#define STACK_H

#include "expression.h"

typedef struct Stack {
    int top;
    int capacity;
    TreeNode** array;
} Stack;

Stack* createStack(int capacity);
int isEmpty(Stack* stack);
void push(Stack* stack, TreeNode* item);
TreeNode* pop(Stack* stack);
TreeNode* peek(Stack* stack);

// CharStack for infix to postfix conversion
typedef struct CharStack {
    int top;
    int capacity;
    char* array;
} CharStack;

CharStack* createCharStack(int capacity);
int isEmptyChar(CharStack* stack);
void pushChar(CharStack* stack, char item);
char popChar(CharStack* stack);
char peekChar(CharStack* stack);
void freeCharStack(CharStack* stack);

#endif // STACK_H
