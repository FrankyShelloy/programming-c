#include "btree.h"
#include <stdlib.h>
#include <string.h>

static Node *create_node(int order, int is_leaf) {
    Node *node = malloc(sizeof(Node));
    if (!node) return NULL;

    node->count = 0;
    node->keys = malloc((2 * order - 1) * sizeof(int));
    if (!node->keys) {
        free(node);
        return NULL;
    }

    node->children = is_leaf ? NULL : calloc(2 * order, sizeof(Node *));
    if (!is_leaf && !node->children) {
        free(node->keys);
        free(node);
        return NULL;
    }

    return node;
}

static void free_node(Node *node) {
    if (!node) return;

    free(node->keys);
    if (node->children) {
        for (int i = 0; i <= node->count; i++) {
            free_node(node->children[i]);
        }
        free(node->children);
    }
    free(node);
}

static int is_leaf(Node *node) {
    return node->children == NULL;
}

static int tree_height(Node *node) {
    if (!node) return 0;
    if (is_leaf(node)) return node->count > 0;

    int height = 0;
    for (int i = 0; i < node->count; i++) {
        height = MAX(height, tree_height(node->children[i]) + 1);
    }
    return height;
}

static int find_index(Node *node, int key) {
    int left = 0, right = node->count - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (key < node->keys[mid]) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return right + 1;
}

static void split_node(Node *parent, int index, int order) {
    Node *child = parent->children[index];
    Node *sibling = create_node(order, is_leaf(child));

    sibling->count = order - 1;
    child->count = order - 1;

    memcpy(sibling->keys, child->keys + order, (order - 1) * sizeof(int));
    if (!is_leaf(child)) {
        memcpy(sibling->children, child->children + order, order * sizeof(Node *));
    }

    memmove(parent->keys + index + 1, parent->keys + index,
            (parent->count - index) * sizeof(int));
    memmove(parent->children + index + 2, parent->children + index + 1,
            (parent->count - index) * sizeof(Node *));

    parent->keys[index] = child->keys[order - 1];
    parent->count++;
    parent->children[index + 1] = sibling;
}

BTree *create_tree(int order) {
    BTree *tree = malloc(sizeof(BTree));
    if (!tree) return NULL;

    tree->order = order;
    tree->root = create_node(order, 1);
    if (!tree->root) {
        free(tree);
        return NULL;
    }
    return tree;
}

void free_tree(BTree *tree) {
    if (!tree) return;
    free_node(tree->root);
    free(tree);
}

static int insert_at_node(BTree *tree, Node *node, int key) {
    if (!node || node->count == 2 * tree->order - 1) return -1;

    int index = find_index(node, key);
    if (is_leaf(node)) {
        memmove(node->keys + index + 1, node->keys + index,
                (node->count - index) * sizeof(int));
        node->keys[index] = key;
        node->count++;
    } else {
        if (node->children[index]->count == 2 * tree->order - 1) {
            split_node(node, index, tree->order);
            index += key > node->keys[index];
        }
        if (insert_at_node(tree, node->children[index], key)) {
            return -1;
        }
    }
    return 0;
}

int insert_key(BTree *tree, int key) {
    if (!tree) return -1;

    if (tree->root->count == 2 * tree->order - 1) {
        Node *new_root = create_node(tree->order, 0);
        new_root->children[0] = tree->root;
        split_node(new_root, 0, tree->order);
        tree->root = new_root;
    }
    return insert_at_node(tree, tree->root, key);
}

int get_height(BTree *tree) {
    return tree ? tree_height(tree->root) : -1;
}