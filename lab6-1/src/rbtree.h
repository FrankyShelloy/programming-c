#ifndef RBTREE_H
#define RBTREE_H

#define RED   0
#define BLACK 1

typedef struct RBNode {
    int val;
    unsigned char color;
    struct RBNode *left, *right;
} RBNode;

void initNode(RBNode *node, int val);
void findKin(RBNode *root, RBNode *node, RBNode **par, RBNode **gpar);
void rotLeft(RBNode **root, RBNode *piv);
void rotRight(RBNode **root, RBNode *piv);
void fixInsert(RBNode **root, RBNode *node);
void addNode(RBNode **root, RBNode *newNode);
int calcBlackHt(RBNode *node);

#endif