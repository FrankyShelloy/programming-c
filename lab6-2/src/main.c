#include <stdio.h>
#include <stdlib.h>

#define MAX_N 2000
#define MAX_KEYS 2000000

typedef struct Node {
    int* keys;
    struct Node** children;
    int num_keys;
    int is_leaf;
} Node;

Node* create_node(int B, int is_leaf) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->keys = (int*)malloc((2 * B - 1) * sizeof(int));
    node->children = (Node**)malloc(2 * B * sizeof(Node*));
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i < 2 * B; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void free_node(Node* node, int B) {
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            if (node->children[i]) free_node(node->children[i], B);
        }
    }
    free(node->keys);
    free(node->children);
    free(node);
}

void split_child(Node* parent, int i, Node* child, int B) {
    Node* new_node = create_node(B, child->is_leaf);
    new_node->num_keys = B - 1;

    for (int j = 0; j < B - 1; j++) {
        new_node->keys[j] = child->keys[j + B];
    }

    if (!child->is_leaf) {
        for (int j = 0; j < B; j++) {
            new_node->children[j] = child->children[j + B];
            child->children[j + B] = NULL;
        }
    }

    child->num_keys = B - 1;

    for (int j = parent->num_keys; j >= i + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[i + 1] = new_node;


    for (int j = parent->num_keys - 1; j >= i; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[i] = child->keys[B - 1];
    parent->num_keys++;
}

void insert_non_full(Node* node, int key, int B) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->num_keys++;
    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;

        if (node->children[i]->num_keys == 2 * B - 1) {
            split_child(node, i, node->children[i], B);
            if (key > node->keys[i]) {
                i++;
            }
        }
        insert_non_full(node->children[i], key, B);
    }
}

Node* insert(Node* root, int key, int B, int* height) {
    if (!root) {
        root = create_node(B, 1);
        root->keys[0] = key;
        root->num_keys = 1;
        *height = 1;
        return root;
    }

    if (root->num_keys == 2 * B - 1) {
        Node* new_root = create_node(B, 0);
        new_root->children[0] = root;
        split_child(new_root, 0, root, B);
        int i = (new_root->keys[0] < key) ? 1 : 0;
        insert_non_full(new_root->children[i], key, B);
        root = new_root;
        (*height)++;
    } else {
        insert_non_full(root, key, B);
    }
    return root;
}

int main() {
    int B, N;

    if (scanf("%d", &B) != 1 || B < 2 || B > MAX_N) {
        printf("invalid input\n");
        return 0;
    }

    if (scanf("%d", &N) != 1 || N < 0 || N > MAX_KEYS) {
        printf("invalid input\n");
        return 0;
    }

    Node* root = NULL;
    int height = 0;

    for (int i = 0; i < N; i++) {
        int key;
        if (scanf("%d", &key) != 1) {
            printf("invalid input\n");
            if (root) free_node(root, B);
            return 0;
        }
        root = insert(root, key, B, &height);
    }

    printf("%d\n", height);

    if (root) free_node(root, B);
    return 0;
}