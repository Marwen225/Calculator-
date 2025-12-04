/*
 * Calculator - Single File Version
 * Calculateur d'expressions mathématiques avec arbre d'expression
 * Compile avec: gcc calculator_single_file.c -o calculator_single
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ============================================
// STRUCTURES ET TYPES
// ============================================

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

typedef struct Stack {
    int top;
    int capacity;
    TreeNode** array;
} Stack;

typedef struct CharStack {
    int top;
    int capacity;
    char* array;
} CharStack;

// ============================================
// UTILITAIRES
// ============================================

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

// ============================================
// GESTION DES PILES (Stack)
// ============================================

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
        return;
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

// ============================================
// GESTION DES PILES DE CARACTÈRES (CharStack)
// ============================================

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

// ============================================
// CONVERSION INFIX -> POSTFIX
// ============================================

char* infixToPostfix(const char* infix) {
    int len = strlen(infix);
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
        if (isspace(infix[i])) {
            i++;
            continue;
        }

        if (isOperand(infix[i])) {
            postfix[k++] = infix[i++];
            postfix[k++] = ' ';
        }
        else if (infix[i] == '(') {
            pushChar(stack, infix[i++]);
        }
        else if (infix[i] == ')') {
            while (!isEmptyChar(stack) && peekChar(stack) != '(') {
                postfix[k++] = popChar(stack);
                postfix[k++] = ' ';
            }
            if (!isEmptyChar(stack) && peekChar(stack) != '(') {
                free(postfix);
                freeCharStack(stack);
                return NULL;
            } else {
                popChar(stack);
            }
            i++;
        }
        else if (isOperator(infix[i])) {
            while (!isEmptyChar(stack) && precedence(peekChar(stack)) >= precedence(infix[i])) {
                postfix[k++] = popChar(stack);
                postfix[k++] = ' ';
            }
            pushChar(stack, infix[i++]);
        }
        else {
            i++;
        }
    }

    while (!isEmptyChar(stack)) {
        postfix[k++] = popChar(stack);
        postfix[k++] = ' ';
    }

    postfix[k] = '\0';
    if (k > 0 && postfix[k-1] == ' ') {
        postfix[k-1] = '\0';
    }

    freeCharStack(stack);
    return postfix;
}

// ============================================
// CONSTRUCTION DE L'ARBRE D'EXPRESSION
// ============================================

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
    
    if (stack) {
        free(stack->array);
        free(stack);
    }

    return root;
}

// ============================================
// SIMPLIFICATION DE L'ARBRE
// ============================================

static void freeTreeInternal(TreeNode* node) {
    if (!node) return;
    freeTreeInternal(node->left);
    freeTreeInternal(node->right);
    free(node);
}

void simplifyTree(TreeNode** nodeRef) {
    if (!nodeRef || !*nodeRef) return;

    TreeNode* node = *nodeRef;

    simplifyTree(&(node->left));
    simplifyTree(&(node->right));

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
                    return; 
                }
                break;
            default: return;
        }

        TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
        newNode->type = CONSTANT;
        newNode->value.constant = result;
        newNode->left = NULL;
        newNode->right = NULL;

        freeTreeInternal(node);
        *nodeRef = newNode;
        return;
    }

    // 2. Identity '+' (x + 0 = x, 0 + x = x)
    if (node->value.operator == '+') {
        if (right && right->type == CONSTANT && right->value.constant == 0.0) {
            *nodeRef = left;
            node->left = NULL; 
            freeTreeInternal(node);
            return;
        }
        if (left && left->type == CONSTANT && left->value.constant == 0.0) {
            *nodeRef = right;
            node->right = NULL;
            freeTreeInternal(node);
            return;
        }
    }

    // 3. Identity '*' (x * 1 = x, 1 * x = x)
    if (node->value.operator == '*') {
        if (right && right->type == CONSTANT && right->value.constant == 1.0) {
            *nodeRef = left;
            node->left = NULL;
            freeTreeInternal(node);
            return;
        }
        if (left && left->type == CONSTANT && left->value.constant == 1.0) {
            *nodeRef = right;
            node->right = NULL;
            freeTreeInternal(node);
            return;
        }
    }

    // 4. Zero Property '*' (x * 0 = 0, 0 * x = 0)
    if (node->value.operator == '*') {
        if ((right && right->type == CONSTANT && right->value.constant == 0.0) ||
            (left && left->type == CONSTANT && left->value.constant == 0.0)) {
            
            TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
            newNode->type = CONSTANT;
            newNode->value.constant = 0.0;
            newNode->left = NULL;
            newNode->right = NULL;

            freeTreeInternal(node);
            *nodeRef = newNode;
            return;
        }
    }
}

// ============================================
// ÉVALUATION DE L'ARBRE
// ============================================

double evaluateTree(TreeNode* root, char varName, double varValue) {
    if (!root) return 0.0;

    if (root->type == CONSTANT) {
        return root->value.constant;
    }

    if (root->type == VARIABLE) {
        if (root->value.variable == varName) {
            return varValue;
        } else {
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

// ============================================
// AFFICHAGE ET LIBÉRATION DE MÉMOIRE
// ============================================

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

void freeTree(TreeNode* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

// ============================================
// FONCTION PRINCIPALE
// ============================================

int main(int argc, char *argv[]) {
    // Valeurs par défaut
    char infix[256] = "(x + 0) * (5 - 3)";
    double x_value = 10.0;
    
    // Analyser les arguments de ligne de commande
    if (argc >= 2) {
        strncpy(infix, argv[1], sizeof(infix) - 1);
        infix[sizeof(infix) - 1] = '\0';
    }
    if (argc >= 3) {
        x_value = atof(argv[2]);
    }
    
    printf("=== Expression Calculator ===\n\n");
    printf("Original infix expression: %s\n\n", infix);
    
    // 1. Convertir en postfix
    char* postfix = infixToPostfix(infix);
    if (!postfix) {
        printf("Error: Failed to convert to postfix\n");
        return 1;
    }
    printf("Postfix expression: %s\n\n", postfix);
    
    // 2. Construire l'arbre d'expression
    char postfixCopy[256];
    strncpy(postfixCopy, postfix, sizeof(postfixCopy));
    postfixCopy[sizeof(postfixCopy) - 1] = '\0';
    
    TreeNode* root = buildExpressionTree(postfixCopy);
    if (!root) {
        printf("Error: Failed to build expression tree\n");
        free(postfix);
        return 1;
    }
    
    // 3. Afficher l'arbre original
    printf("Original tree (inorder): ");
    printInorder(root);
    printf("\n\n");
    
    // 4. Simplifier l'arbre
    simplifyTree(&root);
    
    // 5. Afficher l'arbre simplifié
    printf("Simplified tree (inorder): ");
    printInorder(root);
    printf("\n\n");
    
    // 6. Évaluer l'arbre avec la valeur de x
    printf("Evaluating with x = %.1f:\n", x_value);
    double result = evaluateTree(root, 'x', x_value);
    printf("Result: %.2f\n\n", result);
    
    // 7. Libérer la mémoire
    freeTree(root);
    free(postfix);
    
    printf("=== Calculation Complete ===\n");
    return 0;
}
