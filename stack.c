#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (TreeNode**)malloc(stack->capacity * sizeof(TreeNode*));
    if (!stack->array) {
        free(stack);
        return NULL;
    }
    return stack;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, TreeNode* item) {
    if (stack->top == stack->capacity - 1) {
        return; // Stack overflow, for now just return
    }
    stack->array[++stack->top] = item;
}

TreeNode* pop(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->array[stack->top--];
}

TreeNode* peek(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->array[stack->top];
}

// CharStack Implementation

CharStack* createCharStack(int capacity) {
    CharStack* stack = (CharStack*)malloc(sizeof(CharStack));
    if (!stack) return NULL;
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char*)malloc(stack->capacity * sizeof(char));
    if (!stack->array) {
        free(stack);
        return NULL;
    }
    return stack;
}

int isEmptyChar(CharStack* stack) {
    return stack->top == -1;
}

void pushChar(CharStack* stack, char item) {
    if (stack->top == stack->capacity - 1) {
        return;
    }
    stack->array[++stack->top] = item;
}

char popChar(CharStack* stack) {
    if (isEmptyChar(stack)) {
        return '\0';
    }
    return stack->array[stack->top--];
}

char peekChar(CharStack* stack) {
    if (isEmptyChar(stack)) {
        return '\0';
    }
    return stack->array[stack->top];
}

void freeCharStack(CharStack* stack) {
    if (stack) {
        free(stack->array);
        free(stack);
    }
}
