#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int power(unsigned int base, unsigned int exponent) {
    unsigned int result = 1;
    while (exponent > 0) {
        result *= base;
        exponent--;
    }
    return result;
}

void checkSubstringMatch(const unsigned char *bufferWindow, int windowStart, const char target[], long long matchStartIndex, int targetLength) {
    int targetIndex = 0;
    int bufferIndex = windowStart;

    while (target[targetIndex] != '\0') {
        if ((unsigned char)target[targetIndex] != bufferWindow[bufferIndex]) {
            printf(" %lld", matchStartIndex);
            break;
        }
        printf(" %lld", matchStartIndex);
        if (targetIndex + 1 == targetLength) {
            break;
        }
        matchStartIndex++;
        targetIndex++;
        bufferIndex = (bufferIndex + 1) % targetLength;
    }
}

unsigned int computeCharHash(const unsigned char character, unsigned long long position) {
    return (character % 3) * (unsigned int)power(3, position);
}

unsigned int computeTargetHash(const char target[]) {
    unsigned int hashValue = 0;
    unsigned long targetLength = strlen(target);
    for (unsigned long position = 0; position < targetLength; position++) {
        hashValue += computeCharHash((unsigned char)target[position], position);
    }
    return hashValue;
}

int main() {
    char targetString[18];
    if (!fgets(targetString, sizeof(targetString), stdin)) {
        return 0;
    }
    targetString[strcspn(targetString, "\n")] = '\0';

    unsigned int targetHash = computeTargetHash(targetString);
    int targetLength = strlen(targetString);

    unsigned char *bufferWindow = malloc(sizeof(unsigned char) * targetLength);
    if (!bufferWindow) {
        return 1;
    }

    int windowStart = 0, windowEnd = -1;
    unsigned char inputBuffer[1024];
    long long globalIndex = 0;
    unsigned int currentHash = 0;
    int matchFound = 0;
    const unsigned int highestPowerFactor = (unsigned int)power(3, targetLength - 1);

    while (1) {
        int bytesRead = fread(inputBuffer, 1, sizeof(inputBuffer), stdin);
        if (bytesRead == 0) {
            break;
        }

        for (int bufferIndex = 0; bufferIndex < bytesRead; bufferIndex++) {
            unsigned char currentChar = inputBuffer[bufferIndex];

            if ((int)globalIndex < targetLength) {
                windowEnd = (windowEnd + 1) % targetLength;
                bufferWindow[windowEnd] = currentChar;
                currentHash += computeCharHash(currentChar, globalIndex);
            } else {
                unsigned char outgoingChar = bufferWindow[windowStart];
                windowStart = (windowStart + 1) % targetLength;
                windowEnd = (windowEnd + 1) % targetLength;
                bufferWindow[windowEnd] = currentChar;
                currentHash = ((currentHash - (outgoingChar % 3)) / 3) + highestPowerFactor * (currentChar % 3);
            }

            if (currentHash == targetHash && (int)globalIndex >= targetLength - 1) {
                if (!matchFound) {
                    printf("%u", currentHash);
                }
                long long matchStartIndex = globalIndex - (long long)targetLength + 2;
                if (matchStartIndex - 2 == 0) {
                    matchStartIndex--;
                }
                checkSubstringMatch(bufferWindow, windowStart, targetString, matchStartIndex, targetLength);
                matchFound = 1;
            }
            globalIndex++;
        }
    }

    if (!matchFound) {
        printf("0");
    }
    free(bufferWindow);
    return 0;
}
