#include "huffman.h"
#include <stdlib.h>
#include <string.h>

// Создает новый узел дерева Хаффмана
Node* createNode(int value, size_t frequency, Node* leftChild, Node* rightChild)
{
    Node* node = malloc(sizeof(Node));
    node->value = value;
    node->frequency = frequency;
    node->leftChild = leftChild;
    node->rightChild = rightChild;
    return node;
}

// Создает очередь с приоритетами
PriorityQueue* createPriorityQueue(int maxCapacity)
{
    PriorityQueue* queue = malloc(sizeof(PriorityQueue) + maxCapacity * sizeof(Node*));
    queue->maxCapacity = maxCapacity;
    queue->currentSize = -1;
    return queue;
}

// Возвращает текущий размер очереди
int getQueueSize(PriorityQueue* queue)
{
    return queue->currentSize + 1;
}

// Проверяет, пуста ли очередь
bool isQueueEmpty(PriorityQueue* queue)
{
    return getQueueSize(queue) == 0;
}

// Проверяет, заполнена ли очередь
bool isQueueFull(PriorityQueue* queue)
{
    return getQueueSize(queue) == queue->maxCapacity;
}

// Добавляет узел в очередь с учетом приоритета
bool addToQueue(PriorityQueue* queue, Node* node)
{
    if (isQueueFull(queue)){
        return 1;
    }

    int insertPosition = getQueueSize(queue);
    while (insertPosition > 0 && queue->nodes[insertPosition - 1]->frequency > node->frequency){
        queue->nodes[insertPosition] = queue->nodes[insertPosition - 1];
        insertPosition--;
    }

    queue->nodes[insertPosition] = node;
    queue->currentSize++;
    return 0;
}

// Возвращает первый узел очереди без удаления
Node* getFirstNode(PriorityQueue* queue)
{
    if (isQueueEmpty(queue)){
        return NULL;
    }
    return queue->nodes[0];
}

// Удаляет и возвращает первый узел очереди
Node* removeFirstNode(PriorityQueue* queue)
{
    if (isQueueEmpty(queue)){
        return NULL;
    }

    Node* removed = queue->nodes[0];
    for (int i = 0; i < queue->currentSize; i++){
        queue->nodes[i] = queue->nodes[i + 1];
    }
    queue->currentSize--;
    return removed;
}

// Рекурсивно освобождает память узла и его потомков
void deleteNode(Node* node)
{
    if (node == NULL) return;
    deleteNode(node->leftChild);
    deleteNode(node->rightChild);
    free(node);
}

// Освобождает память очереди и всех узлов
void freeQueue(PriorityQueue* queue)
{
    for (size_t i = 0; i < (size_t)getQueueSize(queue); ++i){
        deleteNode(queue->nodes[i]);
    }
    free(queue);
}

// Рекурсивно строит коды Хаффмана
void buildCode(struct Code* codes, Node* node, struct Code code, unsigned char index)
{
    if (node->leftChild != NULL){
        code.bits[index] = '0';
        buildCode(codes, node->leftChild, code, index + 1);
    }

    if (node->rightChild != NULL){
        code.bits[index] = '1';
        buildCode(codes, node->rightChild, code, index + 1);
    }

    if (node->leftChild == NULL && node->rightChild == NULL){
        codes[node->value] = code;
        codes[node->value].length = index;
        if (index == 0) codes[node->value].length = 1;
        return;
    }
}

// Создает таблицу кодов Хаффмана
struct Code* createCodesTable(Node* root)
{
    struct Code code = {.length = 0, .bits = {'0'}};
    memset(code.bits, '0', sizeof(code.bits));
    struct Code* codes = malloc(sizeof(struct Code) * LETTERS_COUNT);
    for (int i = 0; i < LETTERS_COUNT; ++i){
        codes[i].length = 0;
    }
    buildCode(codes, root, code, 0);
    return codes;
}

// Вычисляет частоту символов в файле
size_t* calculateFrequencies(FILE* inputFile)
{
    unsigned char buffer[BUFSIZ];
    size_t* frequencies = calloc(LETTERS_COUNT, sizeof(size_t));
    while (1){
        size_t bytesRead = fread(buffer, 1, BUFSIZ, inputFile);
        if (bytesRead == 0) break;
        for (size_t i = 0; i < bytesRead; i++){
            frequencies[(unsigned char)buffer[i]]++;
        }
    }
    return frequencies;
}

// Строит дерево Хаффмана
void constructHuffmanTree(PriorityQueue* queue)
{
    while (getQueueSize(queue) > 1){
        Node* firstNode = removeFirstNode(queue);
        Node* secondNode = removeFirstNode(queue);
        Node* combinedNode = createNode(EOF, firstNode->frequency + secondNode->frequency, firstNode, secondNode);
        addToQueue(queue, combinedNode);
    }
}

// Заполняет очередь узлами на основе частот символов
void populateQueueWithLetters(const size_t* frequencies, PriorityQueue* queue)
{
    for (size_t i = 0; i < LETTERS_COUNT; ++i){
        if (frequencies[i] == 0) continue;
        Node* node = createNode((int)i, frequencies[i], NULL, NULL);
        addToQueue(queue, node);
    }
}

// Сериализует дерево Хаффмана в выходной файл
void serializeHuffmanTree(FILE* outputFile, struct Node* node, struct TSmartByte* byte)
{
    if (node->leftChild != NULL || node->rightChild != NULL){
        setBit(byte, 0, outputFile);
        if (node->leftChild != NULL){
            serializeHuffmanTree(outputFile, node->leftChild, byte);
        }
        if (node->rightChild != NULL){
            serializeHuffmanTree(outputFile, node->rightChild, byte);
        }
    }

    if (node->leftChild == NULL && node->rightChild == NULL){
        setBit(byte, 1, outputFile);
        char symbol = (char)node->value;
        for (int i = 0; i < 8; ++i){
            setBit(byte, ((symbol & 128) == 0) ? 0 : 1, outputFile);
            symbol <<= 1;
        }
    }
}

// Кодирует буфер данных с использованием кодов Хаффмана
void encodeBuffer(FILE* outputFile, const unsigned char buffer[BUFSIZ], struct TSmartByte* byte, size_t bytesRead,
                  struct Code* codes)
{
    for (size_t i = 0; i < bytesRead; ++i){
        unsigned char c = (unsigned char)buffer[i];
        size_t length = codes[c].length;
        for (size_t j = 0; j < length; ++j){
            setBit(byte, codes[c].bits[j] - '0', outputFile);
        }
    }
}

// Кодирует данные из входного файла в выходной
void encodeDataToFile(FILE* inputFile, FILE* outputFile, struct TSmartByte* byte, struct Code* codes)
{
    unsigned char buffer[BUFSIZ];
    while (1){
        size_t bytesRead = fread(buffer, 1, BUFSIZ, inputFile);
        if (bytesRead == 0) break;
        encodeBuffer(outputFile, buffer, byte, bytesRead, codes);
    }
}

// Кодирует входной файл с использованием дерева Хаффмана
void encodeFile(FILE* inputFile, struct Node* root, bool isTestingMode, FILE* outputFile)
{
    if (root == NULL){
        return;
    }
    struct Code* codes = createCodesTable(root);
    struct TSmartByte byte = {.occupiedBits = 0};
    long filePosition = ftell(outputFile);
    putc(' ', outputFile);

    if (root->leftChild == NULL && root->rightChild == NULL)
        serializeHuffmanTree(outputFile, root, &byte);
    if (root->leftChild != NULL)
        serializeHuffmanTree(outputFile, root->leftChild, &byte);
    if (root->rightChild != NULL)
        serializeHuffmanTree(outputFile, root->rightChild, &byte);
    flushByte(&byte, outputFile);

    long offset = isTestingMode ? 1 : 0;
    fseek(inputFile, offset, SEEK_SET);

    encodeDataToFile(inputFile, outputFile, &byte, codes);
    char significantBits = (char)(8 - byte.occupiedBits);
    flushByte(&byte, outputFile);
    fseek(outputFile, filePosition, 0);
    putc(significantBits, outputFile);

    free(codes);
}

// Десериализует дерево Хаффмана из входного файла
struct Node* deserializeTree(TBitFile* bitFile, struct TSmartByte* byte, size_t* paddingCount)
{
    int bit = readBit(bitFile);
    if (bit){
        for (int i = 0; i < 8; ++i){
            int currentBit = readBit(bitFile);
            setBit(byte, currentBit, NULL);
        }
        (*paddingCount)++;
        return createNode(byte->data, EOF, NULL, NULL);
    }
    struct Node* node = createNode(EOF, -1, NULL, NULL);
    (*paddingCount)++;
    node->leftChild = deserializeTree(bitFile, byte, paddingCount);
    node->rightChild = deserializeTree(bitFile, byte, paddingCount);
    return node;
}

// Декодирует дерево Хаффмана из входного файла
struct Node* decodeTree(TBitFile* bitFile)
{
    struct Node* root = createNode(EOF, 0, NULL, NULL);
    struct TSmartByte byte = {.occupiedBits = 0};
    size_t paddingCount = 0;
    root->leftChild = deserializeTree(bitFile, &byte, &paddingCount);
    root->rightChild = deserializeTree(bitFile, &byte, &paddingCount);

    if (paddingCount % 8 != 0){
        for (size_t i = 0; i < 8 - (paddingCount % 8); ++i){
            readBit(bitFile);
        }
    }
    byte.occupiedBits = 0;
    return root;
}

// Открывает файл для чтения или записи
FILE* openFile(const char* filePath, bool isWriteMode)
{
    FILE* filePointer;
    if (!isWriteMode){
        filePointer = fopen(filePath, "rb");
    }
    else{
        filePointer = fopen(filePath, "wb");
    }
    return filePointer;
}

// Обходит дерево Хаффмана на основе входного бита
void traverseTree(struct Node** currentNode, int bit, struct Node* root, FILE* outputFile)
{
    if (bit){
        *currentNode = (*currentNode)->rightChild;
    }
    else{
        *currentNode = (*currentNode)->leftChild;
    }
    if ((*currentNode)->leftChild == NULL && (*currentNode)->rightChild == NULL){
        if (outputFile != NULL){
            putc((*currentNode)->value, outputFile);
        }
        else{
            putc((*currentNode)->value, stdout);
        }
        *currentNode = root;
    }
}

// Кодирует входной файл
void encode(FILE* inputFile, bool isTestingMode, FILE* outputFile)
{
    size_t* frequencies = calculateFrequencies(inputFile);
    PriorityQueue* queue = createPriorityQueue(LETTERS_COUNT);
    populateQueueWithLetters(frequencies, queue);
    if (getQueueSize(queue) == 1){
        struct Node* duplicateNode = createNode(getFirstNode(queue)->value, getFirstNode(queue)->frequency,
                                                getFirstNode(queue)->leftChild, getFirstNode(queue)->rightChild);
        addToQueue(queue, duplicateNode);
    }
    constructHuffmanTree(queue);

    struct Node* root = getFirstNode(queue);
    encodeFile(inputFile, root, isTestingMode, outputFile);

    free(frequencies);
    freeQueue(queue);
}

// Возвращает позицию конца файла
long getFileEndPosition(FILE* filePointer)
{
    long currentPosition = ftell(filePointer);
    fseek(filePointer, 0, SEEK_END);
    long endPosition = ftell(filePointer);
    fseek(filePointer, currentPosition, SEEK_SET);
    return endPosition;
}

// Освобождает память дерева Хаффмана
void freeTree(struct Node* root)
{
    if (root != NULL){
        freeTree(root->leftChild);
        freeTree(root->rightChild);
        free(root);
    }
}

// Декодирует входной файл
void decode(FILE* inputFile, FILE* outputFile)
{
    int significantBits = fgetc(inputFile);
    if (significantBits == EOF){
        return;
    }
    TBitFile* bitFile = createBitFile(inputFile);
    long endPosition = getFileEndPosition(inputFile);
    struct Node* root = decodeTree(bitFile);
    struct Node* currentNode = root;

    char bitCount = 0;
    while (1){
        int bit = readBit(bitFile);
        if (EOF == bit) break;
        traverseTree(&currentNode, bit, root, outputFile);
        if (ftell(bitFile->filePointer) == endPosition){
            bitCount++;
            if (bitCount == (8 - significantBits)) break;
        }
    }

    freeTree(root);
    closeBitFile(bitFile);
}
