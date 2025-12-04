#include <ctype.h>
#include "utils.h"

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int precedence(char op) {
    if (op == '*' || op == '/') {
        return 2;
    }
    if (op == '+' || op == '-') {
        return 1;
    }
    return -1;
}

int isOperand(char c) {
    return isalnum(c);
}
