#include <stdio.h>
#include <stdlib.h>

typedef struct AvlTree {
    struct AvlTree *left;
    struct AvlTree *right;
    int value;
    unsigned char height;
} AvlTree;

AvlTree* createTree(int value, int index, AvlTree* mem) {
    AvlTree* t = &mem[index];
    t->value = value;
    t->left = NULL;
    t->right = NULL;
    t->height = 1;
    return t;
}

unsigned char height(AvlTree* t) {
    return t ? t->height : 0;
}

void fixheight(AvlTree* t) {
    unsigned char hl = height(t->left);
    unsigned char hr = height(t->right);
    t->height = (hl > hr ? hl : hr) + 1;
}

int bfactor(AvlTree* t) {
    return t ? height(t->right) - height(t->left) : 0;
}

AvlTree* rotateRight(AvlTree* y) {
    AvlTree* x = y->left;
    y->left = x->right;
    x->right = y;
    fixheight(y);
    fixheight(x);
    return x;
}


AvlTree* rotateLeft(AvlTree* x) {
    AvlTree* y = x->right;
    x->right = y->left;
    y->left = x;
    fixheight(x);
    fixheight(y);
    return y;
}

AvlTree* balance(AvlTree* t, int printRotate) {
    fixheight(t);
    int bf = bfactor(t);

    if (bf == 2) {
        if (bfactor(t->right) < 0) {
            t->right = rotateRight(t->right);
            t = rotateLeft(t);
            if (printRotate) printf("BL ");
        } else {
            t = rotateLeft(t);
            if (printRotate) printf("SL ");
        }
    } else if (bf == -2) {
        if (bfactor(t->left) > 0) {
            t->left = rotateLeft(t->left);
            t = rotateRight(t);
            if (printRotate) printf("BR ");
        } else {
            t = rotateRight(t);
            if (printRotate) printf("SR ");
        }
    }
    return t;
}

AvlTree* insert(AvlTree* t, int value, int index, AvlTree* mem, int printRotate) {
    if (!t) return createTree(value, index, mem);

    if (value < t->value)
        t->left = insert(t->left, value, index, mem, printRotate);
    else
        t->right = insert(t->right, value, index, mem, printRotate);

    return balance(t, printRotate);
}

void freeTree(AvlTree* mem) {
    free(mem);
}

int main() {
    int N, printRotate = 0;

    if (scanf("%d", &N) != 1) {
        printf("Empty input\n");
        return 0;
    }

    if (N < 0 || N > 2000000) {
        printf("Bad input\n");
        return 0;
    }

    printRotate = (N < 100);

    AvlTree* tArray = (AvlTree*)malloc(N * sizeof(AvlTree));
    if (!tArray) {
        return 0;
    }

    AvlTree* root = NULL;
    for (int i = 0; i < N; i++) {
        int num;
        if (scanf("%d", &num) != 1) {
            printf("Bad input\n");
            freeTree(tArray);
            return 0;
        }
        root = insert(root, num, i, tArray, printRotate);
    }

    printf("%d\n", height(root));
    freeTree(tArray);
    return 0;
}
