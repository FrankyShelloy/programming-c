#include "btree.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int order, count;
    if (scanf("%d %d", &order, &count) != 2) return EXIT_SUCCESS;

    BTree *tree = create_tree(order);
    if (!tree) return EXIT_FAILURE;

    for (int i = 0; i < count; i++) {
        int key;
        if (scanf("%d", &key) != 1) break;
        insert_key(tree, key);
    }

    printf("%d\n", get_height(tree));
    free_tree(tree);
    return EXIT_SUCCESS;
}