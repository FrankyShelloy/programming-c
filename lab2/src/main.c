#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define SIZE_PATTERN 12

void badInput() {
    printf("bad input\n");
}

bool isValid(const char *pattern) {
    bool buffer[SIZE_PATTERN] = {false};
    int len = strlen(pattern);

    for (int i = 0; i < len; i++) {
        if (pattern[i] < '0' || pattern[i] > '9') {
            return false;
        }
        int index = pattern[i] - '0';
        if (buffer[index]) {
            return false;
        }
        buffer[index] = true;
    }
    return true;
}

void swap(char *pattern, int i, int j) {
    char temp = pattern[i];
    pattern[i] = pattern[j];
    pattern[j] = temp;
}

bool nextPermutation(char *pattern) {
    int len = strlen(pattern);
    int i = len - 2;

    while (i >= 0 && pattern[i] >= pattern[i + 1]) i--;

    if (i < 0) return false;

    int j = len - 1;
    while (pattern[j] <= pattern[i]) j--;

    swap(pattern, i, j);

    int left = i + 1, right = len - 1;
    while (left < right) {
        swap(pattern, left, right);
        left++;
        right--;
    }
    return true;
}

int generatePermutations(char *pattern, int numberPermutations) {
    if (!isValid(pattern)) {
        badInput();
        return 0;
    }

    for (int i = 0; i < numberPermutations; i++) {
        if (!nextPermutation(pattern)) break;
        printf("%s\n", pattern);
    }
    return 0;
}

int main(void) {
    char pattern[SIZE_PATTERN];
    if (fgets(pattern, SIZE_PATTERN, stdin) == NULL) {
        badInput();
        return 0;
    }

    size_t len = strcspn(pattern, "\n");
    pattern[len] = '\0';

    if (strlen(pattern) > 10) {
        badInput();
        return 0;
    }

    int numberPermutations;
    if (scanf("%d", &numberPermutations) != 1) {
        badInput();
        return 0;
    }

    return generatePermutations(pattern, numberPermutations);
}
