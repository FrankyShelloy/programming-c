#ifndef FUNC_H
#define FUNC_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 512

size_t* prefixFunction(const uint8_t* str, size_t length);

size_t printKMP(const size_t* prefixes, const uint8_t* format,
                const uint8_t* buffer, size_t length, size_t offset,
                size_t needle_length, FILE* out);

void printKMPtail(const size_t* prefixes, const uint8_t* format,
                  size_t format_length, const uint8_t* buffer, size_t length,
                  size_t offset, size_t needle_length, FILE* out);

void printKMPBuffered(const size_t* prefixes, const uint8_t* format,
                      size_t format_length, FILE* in, FILE* out);

#endif // FUNC_H