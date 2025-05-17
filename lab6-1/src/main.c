#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

int main() {
    int cnt;
    scanf("%d", &cnt);

    if (cnt < 0 || cnt > 2000000) {
        printf("Invalid input\n");
        return 1;
    }

    if (cnt == 0) {
        printf("0\n");
        return 0;
    }

    RBNode *nodes = (RBNode *)malloc(cnt * sizeof(RBNode));
    if (!nodes) {
        printf("Memory allocation failed\n");
        return 1;
    }

    RBNode *root = NULL;

    for (int i = 0; i < cnt; i++) {
        int val;
        scanf("%d", &val);
        initNode(&nodes[i], val);
        addNode(&root, &nodes[i]);
    }

    int ht = calcBlackHt(root);
    printf("%d\n", ht);

    free(nodes);
    return 0;
}