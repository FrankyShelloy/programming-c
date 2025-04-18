#include <stdio.h>
#include <string.h>

int precision = 12;
#define bufSize 64

int charToValue(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

char valueToChar(int value) {
    if (value >= 0 && value <= 9)
        return '0' + value;
    if (value >= 10 && value <= 15)
        return 'A' + (value - 10);
    return -1;
}

long long convertIntPartToDecimal(const char* intPart, int baseN) {
    long long result = 0;
    int len = strlen(intPart);
    for (int i = 0; i < len; i++) {
        int value = charToValue(intPart[i]);
        result = result * baseN + value;
    }
    return result;
}

double convertFracPartToDecimal(const char* fracPart, int baseN) {
    double result = 0.0;
    double factor = 1.0 / baseN;
    int len = strlen(fracPart);
    for (int i = 0; i < len; i++) {
        int value = charToValue(fracPart[i]);
        result += value * factor;
        factor /= baseN;
    }
    return result;
}

void convertDecimalIntToBaseK(long long intPart, int baseK, char* result) {
    char buffer[bufSize];
    int index = 0;
    do {
        buffer[index++] = valueToChar(intPart % baseK);
        intPart /= baseK;
    } while (intPart > 0);

    for (int i = 0; i < index; i++)
        result[i] = buffer[index - i - 1];
    result[index] = '\0';
}

void convertDecimalFracToBaseK(double fracPart, int baseK, char* result, int precision) {
    int index = 0;
    while (precision > 0) {
        fracPart *= baseK;
        int digit = (int)fracPart;
        result[index++] = valueToChar(digit);
        fracPart -= digit;
        precision--;
    }
    result[index] = '\0';
}

int validate_number_in_base(const char* number, int baseN) {
    int hasDigitBeforeDot = 0;
    int hasDigitAfterDot = 0;
    int hasDot = 0;

    for (int unsigned i = 0; i < strlen(number); i++) {
        if (number[i] == '.') {
            if (hasDot) return 0;
            hasDot = 1;
            continue;
        }

        int value = charToValue(number[i]);

        if (value == -1 || value >= baseN) {
            return 0;
        }

        if (value >= 0 && value < baseN) {
            if (hasDot) hasDigitAfterDot = 1;
            else hasDigitBeforeDot = 1;
        }
    }

    if (!hasDigitBeforeDot) {
        return 0;
    }

    if (hasDot && !hasDigitAfterDot) {
        return 0;
    }

    return 1;
}

int convertBaseNToBaseK(const char* number, int baseN, int baseK, int precision) {
    char intPart[bufSize], fracPart[bufSize];
    sscanf(number, "%63[^.].%63s", intPart, fracPart);

    long long decimalIntPart = convertIntPartToDecimal(intPart, baseN);
    double decimalFracPart = convertFracPartToDecimal(fracPart, baseN);

    char resultIntPart[bufSize];
    convertDecimalIntToBaseK(decimalIntPart, baseK, resultIntPart);

    char resultFracPart[bufSize];
    convertDecimalFracToBaseK(decimalFracPart, baseK, resultFracPart, precision);

    printf("%s.%s\n", resultIntPart, resultFracPart);
    return 0;
}

int main() {
    char number[bufSize];
    int baseN, baseK;

    if (scanf("%10d" "%10d", &baseN, &baseK) != 2) {
        return 0;
    }

    if (baseN < 2 || baseK < 2){
        printf("bad input");
        return 0;
    }
    if (baseN > 16 || baseK > 16){
        printf("bad input");
        return 0;
    }

    if (scanf("%63s", number) != 1) {
        return 0;
    }

    if (!validate_number_in_base(number, baseN)) {
        printf("bad input\n");
        return 0;
    }

    convertBaseNToBaseK(number, baseN, baseK, precision);
    return 0;
}