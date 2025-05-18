#ifndef TRIE_H
#define TRIE_H

typedef struct Trie Trie;
typedef struct TrieNode TrieNode;

Trie* initTrie(void);
void freeTrie(Trie* trie);
int getWordCount(const Trie* trie);
void insertWord(Trie* trie, const char* word);
void printWordsWithPrefix(Trie* trie, const char* prefix);
void insertWordFromNode(Trie* trie, TrieNode* currentNode, const char* word, int charOffset);
TrieNode* findPrefixNode(Trie* trie, const char* prefix, int* charOffset);

#endif