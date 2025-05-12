#include "bit_operations.h"
#include <stdlib.h>
#include <limits.h>

void writeByte(FILE *file, unsigned char byte) {
    putc(byte, file);
}

void setBit(struct TSmartByte *byte, unsigned char value, FILE *file) {
    if (byte->occupiedBits == 8) {
        byte->occupiedBits = 0;
        if (file != NULL) {
            writeByte(file, byte->data);
        }
    }

    unsigned char mask = 1 << (7 - byte->occupiedBits);
    byte->occupiedBits++;
    if (value == 0) {
        mask = ~mask;
        byte->data = byte->data & mask;
    } else {
        byte->data = byte->data | mask;
    }
}

void flushByte(struct TSmartByte *byte, FILE *file) {
    while (byte->occupiedBits != 8) {
        setBit(byte, 0, file);
    }
    writeByte(file, byte->data);
    byte->occupiedBits = 0;
}

TBitFile *createBitFile(FILE *filePointer) {
    TBitFile *bitFile = malloc(sizeof(TBitFile));
    if (!bitFile) {
        fclose(filePointer);
        return NULL;
    }

    bitFile->filePointer = filePointer;
    bitFile->currentChar = getc(filePointer);
    bitFile->bitPosition = 0;

    return bitFile;
}

void closeBitFile(TBitFile *bitFile) {
    if (!bitFile)
        return;
    fclose(bitFile->filePointer);
    free(bitFile);
}

int readBit(TBitFile *bitFile) {
    if (!bitFile) {
        return -1;
    }

    if (bitFile->bitPosition >= CHAR_BIT) {
        bitFile->currentChar = getc(bitFile->filePointer);
        bitFile->bitPosition = 0;
    }

    if (EOF == bitFile->currentChar)
        return EOF;

    int bit = (int) READ_BIT(bitFile->currentChar, bitFile->bitPosition);
    bitFile->bitPosition++;
    return bit;
}
