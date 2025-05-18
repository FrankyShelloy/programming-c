#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "trienode.h"

struct Trie {
    TrieNode* rootNode;
    int totalWords;
};

Trie* initTrie(void) {
    Trie* newTrie = calloc(1, sizeof(Trie));
    return newTrie;
}

void freeTrie(Trie* trie) {
    destroyNode(trie->rootNode);
    free(trie);
}

int getWordCount(const Trie* trie) {
    return trie->totalWords;
}

void insertWord(Trie* trie, const char* word) {
    if (trie->rootNode == NULL) {
        trie->rootNode = createNode(word, 1);
        trie->totalWords++;
    } else {
        insertWordFromNode(trie, trie->rootNode, word, 0);
    }
}

void insertWordFromNode(Trie* trie, TrieNode* currentNode, const char* word, int charOffset) {
    int charIndex;
    for (charIndex = 0; word[charIndex] != '\0'; charIndex++) {
        if (currentNode->segmentText[charIndex - charOffset] == '\0') {
            TrieNode* nextChild = findChildWithChar(currentNode, word[charIndex]);
            if (nextChild == NULL) {
                addChildNode(currentNode, createNode(word + charIndex, 1));
                trie->totalWords++;
                return;
            }
            currentNode = nextChild;
            charOffset = charIndex;
        }

        if (currentNode->segmentText[charIndex - charOffset] != word[charIndex]) {
            break;
        }
    }

    if (currentNode->segmentText[charIndex - charOffset] == '\0') {
        currentNode->isCompleteWord = 1;
    } else {
        if (word[charIndex] != '\0') {
            splitNodeAtIndex(currentNode, charIndex - charOffset);
            addChildNode(currentNode, createNode(word + charIndex, 1));
            trie->totalWords++;
        } else {
            splitNodeAtIndex(currentNode, charIndex - charOffset);
            currentNode->isCompleteWord = 1;
        }
        trie->totalWords++;
    }
}

TrieNode* findPrefixNode(Trie* trie, const char* prefix, int* charOffset) {
    TrieNode* currentNode = trie->rootNode;
    *charOffset = 0;

    for (int i = 0; prefix[i] != '\0'; i++) {
        if (currentNode->segmentText[i - *charOffset] == '\0') {
            currentNode = findChildWithChar(currentNode, prefix[i]);
            if (currentNode == NULL) return NULL;
            *charOffset = i;
        }

        if (currentNode->segmentText[i - *charOffset] != prefix[i]) {
            return NULL;
        }
    }

    return currentNode;
}

void printWordsWithPrefix(Trie* trie, const char* prefix) {
    int charOffset;
    TrieNode* prefixNode = findPrefixNode(trie, prefix, &charOffset);
    if (prefixNode == NULL) {
        puts("None");
        return;
    }

    char* wordBuffer = malloc(10001);
    if (wordBuffer == NULL) return;

    strncpy(wordBuffer, prefix, charOffset);
    int wordsFound = printWordsFromNode(prefixNode, wordBuffer, charOffset);
    free(wordBuffer);

    if (wordsFound) {
        puts("");
    } else {
        puts("None");
    }
}