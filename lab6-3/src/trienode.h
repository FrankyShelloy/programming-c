#ifndef TRIENODE_H
#define TRIENODE_H

typedef struct TrieNode TrieNode;

struct TrieNode {
    char* segmentText;
    int isCompleteWord;
    TrieNode* nextSibling;
    TrieNode* firstChild;
};

TrieNode* createNode(const char* segmentText, int isCompleteWord);
void destroyNode(TrieNode* node);
void addChildNode(TrieNode* parentNode, TrieNode* childNode);
TrieNode* findChildWithChar(TrieNode* parentNode, char targetChar);
void splitNodeAtIndex(TrieNode* node, int splitIndex);
int printWordsFromNode(TrieNode* node, char* wordBuffer, int bufferDepth);

#endif