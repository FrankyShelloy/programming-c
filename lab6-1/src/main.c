#include <stdio.h>
#include <stdlib.h>

#define RED 0
#define BLACK 1

typedef struct RBTreeNode {
    int value;
    unsigned char color;
    struct RBTreeNode *leftChild, *rightChild;
} RBTreeNode;


void initializeNode(RBTreeNode* node, int value) {
    node->value = value;
    node->color = RED;
    node->leftChild = node->rightChild = NULL;
}


void findRelatives(RBTreeNode* root, RBTreeNode* node, RBTreeNode** parentPtr, RBTreeNode** grandparentPtr) {
    *parentPtr = NULL;
    *grandparentPtr = NULL;
    RBTreeNode* current = root;

    while (current != NULL && current != node) {
        *grandparentPtr = *parentPtr;
        *parentPtr = current;
        if (node->value < current->value) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }
}

void leftRotate(RBTreeNode** root, RBTreeNode* pivot) {
    RBTreeNode* newParent = pivot->rightChild;
    pivot->rightChild = newParent->leftChild;
    newParent->leftChild = pivot;

    RBTreeNode* parent = NULL;
    RBTreeNode* grandparent = NULL;
    findRelatives(*root, pivot, &parent, &grandparent);

    if (parent == NULL) {
        *root = newParent;
    } else if (parent->leftChild == pivot) {
        parent->leftChild = newParent;
    } else {
        parent->rightChild = newParent;
    }
}

void rightRotate(RBTreeNode** root, RBTreeNode* pivot) {
    RBTreeNode* newParent = pivot->leftChild;
    pivot->leftChild = newParent->rightChild;
    newParent->rightChild = pivot;

    RBTreeNode* parent = NULL;
    RBTreeNode* grandparent = NULL;
    findRelatives(*root, pivot, &parent, &grandparent);

    if (parent == NULL) {
        *root = newParent;
    } else if (parent->leftChild == pivot) {
        parent->leftChild = newParent;
    } else {
        parent->rightChild = newParent;
    }
}


void fixInsertion(RBTreeNode** root, RBTreeNode* node) {
    RBTreeNode* parent = NULL;
    RBTreeNode* grandparent = NULL;

    while (node != *root && node->color == RED) {
        findRelatives(*root, node, &parent, &grandparent);

        if (parent == NULL || parent->color == BLACK) break;

        if (grandparent == NULL) break;

        int parentIsLeft = (parent == grandparent->leftChild);
        RBTreeNode* uncle = parentIsLeft ? grandparent->rightChild : grandparent->leftChild;

        if (uncle != NULL && uncle->color == RED) {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            node = grandparent;
        } else {
            if (parentIsLeft) {
                if (node == parent->rightChild) {
                    node = parent;
                    leftRotate(root, node);
                    findRelatives(*root, node, &parent, &grandparent);
                }
                if (parent && grandparent) {
                    parent->color = BLACK;
                    grandparent->color = RED;
                    rightRotate(root, grandparent);
                }
            } else {
                if (node == parent->leftChild) {
                    node = parent;
                    rightRotate(root, node);
                    findRelatives(*root, node, &parent, &grandparent);
                }
                if (parent && grandparent) {
                    parent->color = BLACK;
                    grandparent->color = RED;
                    leftRotate(root, grandparent);
                }
            }
            break;
        }
    }
    (*root)->color = BLACK;
}

void insertNode(RBTreeNode** root, RBTreeNode* newNode) {
    RBTreeNode* current = *root;
    RBTreeNode* previous = NULL;

    while (current != NULL) {
        previous = current;
        if (newNode->value < current->value) {
            current = current->leftChild;
        } else {
            current = current->rightChild;
        }
    }

    if (previous == NULL) {
        *root = newNode;
    } else if (newNode->value < previous->value) {
        previous->leftChild = newNode;
    } else {
        previous->rightChild = newNode;
    }

    fixInsertion(root, newNode);
}

int calculateBlackHeight(RBTreeNode* node) {
    if (node == NULL)
        return 1;

    int leftHeight = calculateBlackHeight(node->leftChild);
    int rightHeight = calculateBlackHeight(node->rightChild);

    if (leftHeight != rightHeight)
        return -1;

    return leftHeight + (node->color == BLACK ? 1 : 0);
}

int main() {
    int count;
    scanf("%d", &count);

    if (count < 0 || count > 2000000) {
        printf("Invalid input\n");
        return 1;
    }

    if (count == 0) {
        printf("0\n");
        return 0;
    }

    RBTreeNode* nodes = (RBTreeNode*)malloc(count * sizeof(RBTreeNode));
    if (nodes == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    RBTreeNode* root = NULL;

    for (int i = 0; i < count; i++) {
        int value;
        scanf("%d", &value);
        initializeNode(&nodes[i], value);
        insertNode(&root, &nodes[i]);
    }

    int height = calculateBlackHeight(root);
    printf("%d\n", height);

    free(nodes);
    return 0;
}