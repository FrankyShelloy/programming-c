#ifndef STACK_OPERATIONS_H
#define STACK_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 10000
#define STACK_SIZE 10000

typedef struct {
    char data[STACK_SIZE];
    int upper;
} OperationsStack;

void initializationOperationsStack(OperationsStack* stack);
int isEmptyOperationsStack(OperationsStack* stack);
void pushOperationsStack(OperationsStack* stack, char currentOperation);
char popOperationsStack(OperationsStack* stack);
char viewOperationsStack(OperationsStack* stack);

#endif