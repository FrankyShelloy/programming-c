#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

void initNode(RBNode *node, int val) {
    node->val = val;
    node->color = RED;
    node->left = node->right = NULL;
}

void findKin(RBNode *root, RBNode *node, RBNode **par, RBNode **gpar) {
    *par = NULL;
    *gpar = NULL;
    RBNode *cur = root;

    while (cur && cur != node) {
        *gpar = *par;
        *par = cur;
        cur = (node->val < cur->val) ? cur->left : cur->right;
    }
}

void rotLeft(RBNode **root, RBNode *piv) {
    RBNode *newPar = piv->right;
    piv->right = newPar->left;
    newPar->left = piv;

    RBNode *par = NULL;
    RBNode *gpar = NULL;
    findKin(*root, piv, &par, &gpar);

    if (!par) {
        *root = newPar;
    } else if (par->left == piv) {
        par->left = newPar;
    } else {
        par->right = newPar;
    }
}

void rotRight(RBNode **root, RBNode *piv) {
    RBNode *newPar = piv->left;
    piv->left = newPar->right;
    newPar->right = piv;

    RBNode *par = NULL;
    RBNode *gpar = NULL;
    findKin(*root, piv, &par, &gpar);

    if (!par) {
        *root = newPar;
    } else if (par->left == piv) {
        par->left = newPar;
    } else {
        par->right = newPar;
    }
}

void fixInsert(RBNode **root, RBNode *node) {
    RBNode *par = NULL;
    RBNode *gpar = NULL;

    while (node != *root && node->color == RED) {
        findKin(*root, node, &par, &gpar);

        if (!par || par->color == BLACK) break;
        if (!gpar) break;

        int parIsLeft = (par == gpar->left);
        RBNode *uncle = parIsLeft ? gpar->right : gpar->left;

        if (uncle && uncle->color == RED) {
            par->color = BLACK;
            uncle->color = BLACK;
            gpar->color = RED;
            node = gpar;
        } else {
            if (parIsLeft) {
                if (node == par->right) {
                    node = par;
                    rotLeft(root, node);
                    findKin(*root, node, &par, &gpar);
                }
                if (par && gpar) {
                    par->color = BLACK;
                    gpar->color = RED;
                    rotRight(root, gpar);
                }
            } else {
                if (node == par->left) {
                    node = par;
                    rotRight(root, node);
                    findKin(*root, node, &par, &gpar);
                }
                if (par && gpar) {
                    par->color = BLACK;
                    gpar->color = RED;
                    rotLeft(root, gpar);
                }
            }
            break;
        }
    }
    (*root)->color = BLACK;
}

void addNode(RBNode **root, RBNode *newNode) {
    RBNode *cur = *root;
    RBNode *prev = NULL;

    while (cur) {
        prev = cur;
        cur = (newNode->val < cur->val) ? cur->left : cur->right;
    }

    if (!prev) {
        *root = newNode;
    } else if (newNode->val < prev->val) {
        prev->left = newNode;
    } else {
        prev->right = newNode;
    }

    fixInsert(root, newNode);
}

int calcBlackHt(RBNode *node) {
    if (!node) return 1;

    int leftHt = calcBlackHt(node->left);
    int rightHt = calcBlackHt(node->right);

    if (leftHt != rightHt) return -1;

    return leftHt + (node->color == BLACK ? 1 : 0);
}