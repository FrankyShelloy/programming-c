#include "stack_operations.h"

void initializationOperationsStack(OperationsStack* stack) {
    stack->upper = -1;
}

int isEmptyOperationsStack(OperationsStack* stack) {
    return stack->upper == -1;
}

void pushOperationsStack(OperationsStack* stack, char currentOperation) {
    if (stack->upper >= STACK_SIZE - 1) {
        printf("Stack overflow\n");
        exit(1);
    }
    stack->data[++stack->upper] = currentOperation;
}

char popOperationsStack(OperationsStack* stack) {
    if (isEmptyOperationsStack(stack)) {
        printf("Stack underflow\n");
        exit(1);
    }
    return stack->data[stack->upper--];
}

char viewOperationsStack(OperationsStack* stack) {
    if (isEmptyOperationsStack(stack)) {
        return '\0';
    }
    return stack->data[stack->upper];
}