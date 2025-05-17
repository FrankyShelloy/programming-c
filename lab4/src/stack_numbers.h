#ifndef STACK_NUMBERS_H
#define STACK_NUMBERS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stack_operations.h"

typedef struct {
    int data[STACK_SIZE];
    int upper;
} NumberStack;

void initializationNumberStack(NumberStack* stack);
int isEmptyNumberStack(NumberStack* stack);
void pushNumberStack(NumberStack* stack, int value);
int popNumberStack(NumberStack* stack);

int isOperation(char currentChar);
int checkValue(char operation);
int countOneOperation(int num1, int num2, char operation);
void infixToPostfix(char* expression, char* reversePolish);
int countAnswer(char* reversePolish);

#endif