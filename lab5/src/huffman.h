#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include "bit_operations.h"

#define LETTERS_COUNT 256

typedef struct Node {
    int value;
    size_t frequency;
    struct Node *leftChild;
    struct Node *rightChild;
} Node;

typedef struct PriorityQueue {
    int maxCapacity;
    int currentSize;
    Node *nodes[];
} PriorityQueue;

struct Code {
    size_t length;
    unsigned char bits[256];
};

Node *createNode(int value, size_t frequency, Node *leftChild, Node *rightChild);
PriorityQueue *createPriorityQueue(int maxCapacity);
int getQueueSize(PriorityQueue *queue);
bool isQueueEmpty(PriorityQueue *queue);
bool isQueueFull(PriorityQueue *queue);
bool addToQueue(PriorityQueue *queue, Node *node);
Node *getFirstNode(PriorityQueue *queue);
Node *removeFirstNode(PriorityQueue *queue);
void deleteNode(Node *node);
void freeQueue(PriorityQueue *queue);
void buildCode(struct Code *codes, Node *node, struct Code code, unsigned char index);
struct Code *createCodesTable(Node *root);
size_t *calculateFrequencies(FILE *inputFile);
void constructHuffmanTree(PriorityQueue *queue);
void populateQueueWithLetters(const size_t *frequencies, PriorityQueue *queue);
void serializeHuffmanTree(FILE *outputFile, struct Node *node, struct TSmartByte *byte);
void encodeBuffer(FILE *outputFile, const unsigned char buffer[BUFSIZ], struct TSmartByte *byte, size_t bytesRead,
                 struct Code *codes);
void encodeDataToFile(FILE *inputFile, FILE *outputFile, struct TSmartByte *byte, struct Code *codes);
void encodeFile(FILE *inputFile, struct Node *root, bool isTestingMode, FILE *outputFile);
struct Node *deserializeTree(TBitFile *bitFile, struct TSmartByte *byte, size_t *paddingCount);
struct Node *decodeTree(TBitFile *bitFile);
FILE *openFile(const char *filePath, bool isWriteMode);
void traverseTree(struct Node **currentNode, int bit, struct Node *root, FILE *outputFile);
void encode(FILE *inputFile, bool isTestingMode, FILE *outputFile);
long getFileEndPosition(FILE *filePointer);
void freeTree(struct Node *root);
void decode(FILE *inputFile, FILE *outputFile);

#endif
