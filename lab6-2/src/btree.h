#ifndef BTREE_H
#define BTREE_H

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct Node {
    int count;
    int *keys;
    struct Node **children;
} Node;

typedef struct BTree {
    int order;
    Node *root;
} BTree;

BTree *create_tree(int order);
void free_tree(BTree *tree);
int insert_key(BTree *tree, int key);
int get_height(BTree *tree);

#endif