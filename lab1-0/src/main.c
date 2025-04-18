#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256
#define MAX_PATTERN_LENGTH 18


void prepareBadCharTable(unsigned int *badCharTable, const char *pattern, unsigned int patternLength) {
    for (unsigned int i = 0; i < ALPHABET_SIZE; i++) {
        badCharTable[i] = patternLength;
    }
    for (unsigned int i = 0; i < patternLength - 1; i++) {
        badCharTable[(unsigned char)(pattern[i])] = patternLength - i - 1;
    }
}

void checkBySymbols(const unsigned char *textBuffer, size_t bufferIndex, const unsigned char *pattern, long long startIndex, unsigned long patternLength) {
    size_t patternIndex = bufferIndex;
    for (int i = (int)patternLength - 1; i >= 0; i--) {
        printf("%lld ", startIndex);
        if (pattern[i] != textBuffer[patternIndex]) {
            break;
        }
        patternIndex--;
        startIndex--;
    }
}

void boyerMoore(const char *pattern, unsigned int patternLength, unsigned int *badCharTable) {
    unsigned char buffer[BUFSIZ];
    unsigned int skipLimit = patternLength;
    unsigned int skipCounter = 1;
    unsigned int globalCounter = 1;

    while (1) {
        size_t bytesRead = fread(buffer, 1, BUFSIZ, stdin);
        if (bytesRead == 0) break;

        for (size_t bufferIndex = 0; bufferIndex < bytesRead; bufferIndex++) {
            unsigned char currentChar = buffer[bufferIndex];

            if (skipCounter == skipLimit && skipCounter != 0) {
                checkBySymbols(buffer, bufferIndex, (const unsigned char *)pattern, globalCounter, patternLength);
                skipCounter = 1;
                skipLimit = badCharTable[currentChar];
            } else {
                skipCounter++;
            }

            globalCounter++;

            if (bufferIndex < patternLength - 1) continue;
        }
    }
}

int main(void) {
    char pattern[MAX_PATTERN_LENGTH];
    if (!fgets(pattern, MAX_PATTERN_LENGTH, stdin)) {
        return 0;
    }
    pattern[strcspn(pattern, "\n")] = 0;
    unsigned int patternLength = strlen(pattern);

    unsigned int *badCharTable = malloc(ALPHABET_SIZE * sizeof(unsigned int));
    if (badCharTable == NULL) {
        return 1;
    }
    prepareBadCharTable(badCharTable, pattern, patternLength);

    boyerMoore(pattern, patternLength, badCharTable);
    
    free(badCharTable);
    return 0;
}
