#include <stdio.h>
#include "trie.h"

int main(void) {
    int wordCount;
    if (scanf("%d", &wordCount) != 1 || wordCount == 0) {
        puts("None\n0");
        return 0;
    }

    Trie* trie = initTrie();
    char inputWord[10001];

    for (int i = 0; i < wordCount; ++i) {
        if (scanf("%10000s", inputWord) != 1) break;
        insertWord(trie, inputWord);
    }

    char searchPrefix[10001];
    if (scanf("%10000s", searchPrefix) == 1) {
        printWordsWithPrefix(trie, searchPrefix);
        printf("%d\n", getWordCount(trie));
    }

    freeTrie(trie);
    return 0;
}