#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H
#include <stdio.h>

#define READ_BIT(A, B) ((A >> (7 - (B))) & 1)

struct TSmartByte {
    unsigned char data;
    unsigned char occupiedBits;
};

typedef struct {
    FILE *filePointer;
    int currentChar;
    int bitPosition;
} TBitFile;

void writeByte(FILE *file, unsigned char byte);
void setBit(struct TSmartByte *byte, unsigned char value, FILE *file);
void flushByte(struct TSmartByte *byte, FILE *file);
TBitFile *createBitFile(FILE *filePointer);
void closeBitFile(TBitFile *bitFile);
int readBit(TBitFile *bitFile);

#endif
