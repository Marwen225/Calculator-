#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "converter.h"
#include "stack.h"
#include "utils.h"

char* infixToPostfix(const char* infix) {
    int len = strlen(infix);
    // Allocate memory for postfix expression.
    // In worst case (e.g. A+B), length is same + spaces.
    // Let's allocate 2 * len + 1 to be safe for spaces and null terminator.
    char* postfix = (char*)malloc((2 * len + 1) * sizeof(char));
    if (!postfix) return NULL;

    CharStack* stack = createCharStack(len);
    if (!stack) {
        free(postfix);
        return NULL;
    }

    int i = 0;
    int k = 0;
    while (infix[i] != '\0') {
        // Skip whitespace
        if (isspace(infix[i])) {
            i++;
            continue;
        }

        // If operand, add to output
        if (isOperand(infix[i])) {
            postfix[k++] = infix[i++];
            // Handle multi-digit numbers or multi-char variables if needed?
            // User spec says "chiffre ou lettre", implies single char tokens or simple parsing.
            // But usually numbers can be multi-digit. Let's stick to single char for now based on "isOperand"
            // which checks isalnum.
            // Wait, "Gérer les espaces dans l'expression de sortie pour séparer les tokens."
            // So we should add a space after each operand.
            postfix[k++] = ' ';
        }
        // If '(', push to stack
        else if (infix[i] == '(') {
            pushChar(stack, infix[i++]);
        }
        // If ')', pop and output until '('
        else if (infix[i] == ')') {
            while (!isEmptyChar(stack) && peekChar(stack) != '(') {
                postfix[k++] = popChar(stack);
                postfix[k++] = ' ';
            }
            if (!isEmptyChar(stack) && peekChar(stack) != '(') {
                // Invalid expression
                free(postfix);
                freeCharStack(stack);
                return NULL;
            } else {
                popChar(stack); // Pop '('
            }
            i++;
        }
        // If operator
        else if (isOperator(infix[i])) {
            while (!isEmptyChar(stack) && precedence(peekChar(stack)) >= precedence(infix[i])) {
                postfix[k++] = popChar(stack);
                postfix[k++] = ' ';
            }
            pushChar(stack, infix[i++]);
        }
        else {
            // Invalid character
            i++;
        }
    }

    // Pop remaining operators
    while (!isEmptyChar(stack)) {
        postfix[k++] = popChar(stack);
        postfix[k++] = ' ';
    }

    postfix[k] = '\0';
    // Trim trailing space if any
    if (k > 0 && postfix[k-1] == ' ') {
        postfix[k-1] = '\0';
    }

    freeCharStack(stack);
    return postfix;
}
