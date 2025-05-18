#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main() {
    int N, W;
    if (scanf("%d %d", &N, &W) != 2) {
        fprintf(stderr, "Input error\n");
        return 1;
    }

    Item *items = (Item *)malloc(N * sizeof(Item));
    if (!items) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        if (scanf("%d %d", &items[i].weight, &items[i].value) != 2) {
            fprintf(stderr, "Item input error\n");
            free(items);
            return 1;
        }
        items[i].index = i + 1;
    }

    int **dp = (int **)malloc((N + 1) * sizeof(int *));
    if (!dp) {
        fprintf(stderr, "Memory allocation error\n");
        free(items);
        return 1;
    }

    for (int i = 0; i <= N; ++i) {
        dp[i] = (int *)malloc((W + 1) * sizeof(int));
        if (!dp[i]) {
            for (int j = 0; j < i; ++j) free(dp[j]);
            free(dp);
            free(items);
            return 1;
        }
        for (int w = 0; w <= W; ++w) {
            dp[i][w] = 0;
        }
    }

    int *selected = (int *)calloc(N, sizeof(int));
    if (!selected) {
        for (int i = 0; i <= N; ++i) free(dp[i]);
        free(dp);
        free(items);
        return 1;
    }

    knapsack(items, N, W, dp, selected);

    printf("%d\n", get_total_value(dp, N, W));

    for (int i = 0; i < N; ++i) {
        if (selected[i]) {
            printf("%d %d\n", items[i].weight, items[i].value);
        }
    }

    for (int i = 0; i <= N; ++i) free(dp[i]);
    free(dp);
    free(items);
    free(selected);

    return 0;
}
