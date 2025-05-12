#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    FILE* in = stdin;
    FILE* out = stdout;

    char format[17] = {0};
    if (fscanf(in, "%16[^\n]\n", format) != 1) {
        return EXIT_SUCCESS;
    }
    size_t format_length = strlen(format);
    if (format_length == 0) {
        return EXIT_SUCCESS;
    }

    size_t* prefixes = prefixFunction((uint8_t*)format, format_length);
    if (prefixes == NULL) {
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < format_length; i++) {
        fprintf(out, "%zu ", prefixes[i]);
    }
    fprintf(out, "\n");

    printKMPBuffered(prefixes, (uint8_t*)format, format_length, in, out);

    free(prefixes);
    return EXIT_SUCCESS;
}