#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trienode.h"

TrieNode* createNode(const char* segmentText, int isCompleteWord) {
    TrieNode* newNode = calloc(1, sizeof(TrieNode));
    if (newNode == NULL) return NULL;

    newNode->segmentText = malloc(strlen(segmentText) + 1);
    if (newNode->segmentText == NULL) {
        free(newNode);
        return NULL;
    }

    strcpy(newNode->segmentText, segmentText);
    newNode->isCompleteWord = isCompleteWord;
    return newNode;
}

void destroyNode(TrieNode* node) {
    if (node == NULL) return;

    free(node->segmentText);
    destroyNode(node->firstChild);
    destroyNode(node->nextSibling);
    free(node);
}

void addChildNode(TrieNode* parentNode, TrieNode* childNode) {
    if (parentNode->firstChild == NULL) {
        parentNode->firstChild = childNode;
    } else {
        TrieNode* currentChild = parentNode->firstChild;
        while (currentChild->nextSibling != NULL) {
            currentChild = currentChild->nextSibling;
        }
        currentChild->nextSibling = childNode;
    }
}

TrieNode* findChildWithChar(TrieNode* parentNode, char targetChar) {
    TrieNode* currentChild = parentNode->firstChild;
    while (currentChild != NULL) {
        if (currentChild->segmentText[0] == targetChar) {
            return currentChild;
        }
        currentChild = currentChild->nextSibling;
    }
    return NULL;
}

void splitNodeAtIndex(TrieNode* node, int splitIndex) {
    TrieNode* newChildNode = createNode(node->segmentText + splitIndex, node->isCompleteWord);
    if (newChildNode == NULL) return;

    node->segmentText[splitIndex] = '\0';
    newChildNode->firstChild = node->firstChild;
    node->firstChild = newChildNode;
    node->isCompleteWord = 0;
}

int printWordsFromNode(TrieNode* node, char* wordBuffer, int bufferDepth) {
    if (node == NULL) return 0;

    int wordsFound = 0;
    strcpy(wordBuffer + bufferDepth, node->segmentText);
    if (node->isCompleteWord) {
        printf("%s ", wordBuffer);
        wordsFound = 1;
    }

    int segmentLength = strlen(node->segmentText);
    TrieNode* childNode = node->firstChild;
    while (childNode != NULL) {
        wordsFound |= printWordsFromNode(childNode, wordBuffer, bufferDepth + segmentLength);
        childNode = childNode->nextSibling;
    }

    return wordsFound;
}