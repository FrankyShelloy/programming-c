#include "func.h"
#include <stdlib.h>
#include <string.h>

size_t* prefixFunction(const uint8_t* str, size_t length) {
    size_t* p = malloc(length * sizeof(size_t));
    if (p == NULL) {
        return NULL;
    }
    p[0] = 0;
    for (size_t i = 1; i < length; i++) {
        size_t k = p[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = p[k - 1];
        }
        if (str[i] == str[k]) {
            k++;
        }
        p[i] = k;
    }
    return p;
}

size_t printKMP(const size_t* prefixes, const uint8_t* format,
                const uint8_t* buffer, size_t length, size_t offset,
                size_t needle_length, FILE* out) {
    for (size_t i = 0; i < length; i++) {
        if (needle_length > 0 && buffer[i] != format[needle_length]) {
            fprintf(out, "%zu %zu ", offset + i - needle_length + 1, needle_length);
        }
        while (needle_length > 0 && buffer[i] != format[needle_length]) {
            needle_length = prefixes[needle_length - 1];
        }
        if (buffer[i] == format[needle_length]) {
            needle_length++;
        }
    }
    return needle_length;
}

void printKMPtail(const size_t* prefixes, const uint8_t* format,
                  size_t format_length, const uint8_t* buffer, size_t length,
                  size_t offset, size_t needle_length, FILE* out) {
    for (size_t i = 0; i < length; i++) {
        if (needle_length > 0 && buffer[i] != format[needle_length] &&
            i - needle_length + 1 + format_length <= length) {
            fprintf(out, "%zu %zu ", offset + i - needle_length + 1, needle_length);
        }
        while (needle_length > 0 && buffer[i] != format[needle_length]) {
            needle_length = prefixes[needle_length - 1];
        }
        if (buffer[i] == format[needle_length]) {
            needle_length++;
        }
    }
    if (format_length == needle_length) {
        fprintf(out, "%zu %zu ", offset + length - needle_length + 1, needle_length);
    }
}

void printKMPBuffered(const size_t* prefixes, const uint8_t* format,
                      size_t format_length, FILE* in, FILE* out) {
    uint8_t* buffer = malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        return;
    }
    size_t needle_length = 0, offset = 0, length;
    if (format_length > 1) {
        size_t initial_read = fread(buffer, 1, format_length - 1, in);
        if (initial_read < format_length - 1) {
            free(buffer);
            return;
        }
    }
    while ((length = fread(buffer + format_length - 1, 1, BUFFER_SIZE - format_length + 1, in)) > 0) {
        needle_length = printKMP(prefixes, format, buffer, length, offset, needle_length, out);
        offset += length;
        memmove(buffer, buffer + length, format_length - 1);
    }
    printKMPtail(prefixes, format, format_length, buffer + length, format_length - 1, offset, needle_length, out);
    fprintf(out, "\n");
    free(buffer);
}