#include "stack_numbers.h"

void initializationNumberStack(NumberStack* stack) {
    stack->upper = -1;
}

int isEmptyNumberStack(NumberStack* stack) {
    return stack->upper == -1;
}

void pushNumberStack(NumberStack* stack, int value) {
    if (stack->upper >= STACK_SIZE - 1) {
        printf("Stack overflow\n");
        exit(1);
    }
    stack->data[++stack->upper] = value;
}

int popNumberStack(NumberStack* stack) {
    if (isEmptyNumberStack(stack)) {
        printf("Stack underflow\n");
        exit(1);
    }
    return stack->data[stack->upper--];
}

int isOperation(char currentChar) {
    return currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/';
}

int checkValue(char operation) {
    if (operation == '+' || operation == '-') {
        return 1;
    }
    if (operation == '*' || operation == '/') {
        return 2;
    }
    return 0;
}

int countOneOperation(int num1, int num2, char operation) {
    if (operation == '+') {
        return num1 + num2;
    }
    else if (operation == '-') {
        return num1 - num2;
    }
    else if (operation == '*') {
        return num1 * num2;
    }
    else if (operation == '/') {
        if (num2 == 0) {
            printf("division by zero\n");
            exit(0);
        }
        return num1 / num2;
    }
    return 0;
}

void infixToPostfix(char* expression, char* reversePolish) {
    OperationsStack operationStack;
    initializationOperationsStack(&operationStack);

    int i = 0, j = 0;
    while (expression[i]) {
        char currentChar = expression[i];

        if (isdigit(currentChar)) {
            while (isdigit(expression[i])) {
                reversePolish[j++] = expression[i++];
            }
            reversePolish[j++] = ' ';
            i--;
        }
        else if (isOperation(currentChar)) {
            if (i > 0 && isOperation(expression[i - 1])) {
                printf("syntax error\n");
                exit(0);
            }
            while (!isEmptyOperationsStack(&operationStack) && checkValue(viewOperationsStack(&operationStack)) >= checkValue(currentChar)) {
                reversePolish[j++] = popOperationsStack(&operationStack);
            }
            pushOperationsStack(&operationStack, currentChar);
        }
        else if (currentChar == '(') {
            pushOperationsStack(&operationStack, currentChar);
        }
        else if (currentChar == ')') {
            int numberOfOperations = 0;
            int k = i - 1;
            while (k >= 0 && expression[k] != '(') {
                if (isOperation(expression[k])) {
                    numberOfOperations = 1;
                    break;
                }
                k--;
            }

            if (!numberOfOperations && !isdigit(expression[k + 1])) {
                printf("syntax error\n");
                exit(0);
            }

            while (!isEmptyOperationsStack(&operationStack) && viewOperationsStack(&operationStack) != '(') {
                reversePolish[j++] = popOperationsStack(&operationStack);
            }

            if (isEmptyOperationsStack(&operationStack) || viewOperationsStack(&operationStack) != '(') {
                printf("syntax error\n");
                exit(0);
            }

            popOperationsStack(&operationStack);
        }
        else if (currentChar != ' ' && currentChar != '\n') {
            printf("syntax error\n");
            exit(0);
        }
        i++;
    }

    while (!isEmptyOperationsStack(&operationStack)) {
        if (viewOperationsStack(&operationStack) == '(') {
            printf("syntax error\n");
            exit(0);
        }
        reversePolish[j++] = popOperationsStack(&operationStack);
    }

    reversePolish[j] = '\0';
}

int countAnswer(char* reversePolish) {
    NumberStack numberStack;
    initializationNumberStack(&numberStack);

    int cnt = 0;
    while (reversePolish[cnt]) {
        char currentChar = reversePolish[cnt];

        if (isdigit(currentChar)) {
            int num = 0;
            while (isdigit(reversePolish[cnt])) {
                num = num * 10 + (reversePolish[cnt++] - '0');
            }
            pushNumberStack(&numberStack, num);
        }
        else if (isOperation(currentChar)) {
            if (isEmptyNumberStack(&numberStack)) {
                printf("syntax error\n");
                exit(0);
            }

            int num1 = popNumberStack(&numberStack);
            if (isEmptyNumberStack(&numberStack)) {
                printf("syntax error\n");
                exit(0);
            }

            int num2 = popNumberStack(&numberStack);
            pushNumberStack(&numberStack, countOneOperation(num2, num1, currentChar));
        }
        cnt++;
    }

    if (numberStack.upper != 0) {
        printf("syntax error\n");
        exit(0);
    }

    return popNumberStack(&numberStack);
}