#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "stack_operations.h"
#include "stack_numbers.h"

int main() {
    char expression[MAX_LEN];
    char reversePolish[MAX_LEN];

    if (fgets(expression, MAX_LEN, stdin) == NULL) {
        printf("syntax error\n");
        return 0;
    }

    expression[strcspn(expression, "\n")] = '\0';
    if (strlen(expression) == 0) {
        printf("syntax error\n");
        return 0;
    }

    infixToPostfix(expression, reversePolish);
    int answer = countAnswer(reversePolish);
    printf("%d\n", answer);

    return 0;
}