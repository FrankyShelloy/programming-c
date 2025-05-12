#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int weight;
    int value;
    int index;
} Item;

int main() {
    int N, W;
    scanf("%d %d", &N, &W);

    Item *items = (Item *)malloc(N * sizeof(Item));
    for (int i = 0; i < N; ++i) {
        scanf("%d %d", &items[i].weight, &items[i].value);
        items[i].index = i + 1;
    }

    int **dp = (int **)malloc((N + 1) * sizeof(int *));
    for (int i = 0; i <= N; ++i) {
        dp[i] = (int *)malloc((W + 1) * sizeof(int));
        for (int w = 0; w <= W; ++w) {
            dp[i][w] = 0;
        }
    }

    for (int i = 1; i <= N; ++i) {
        for (int w = 0; w <= W; ++w) {
            if (items[i-1].weight <= w) {
                dp[i][w] = (dp[i-1][w] > dp[i-1][w - items[i-1].weight] + items[i-1].value)
                          ? dp[i-1][w]
                          : dp[i-1][w - items[i-1].weight] + items[i-1].value;
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    int total_value = dp[N][W];
    printf("%d\n", total_value);

    int w = W;
    int *selected = (int *)calloc(N, sizeof(int));
    for (int i = N; i > 0; --i) {
        if (dp[i][w] != dp[i-1][w]) {
            selected[i-1] = 1;
            w -= items[i-1].weight;
        }
    }


    for (int i = 0; i < N; ++i) {
        if (selected[i]) {
            printf("%d %d\n", items[i].weight, items[i].value);
        }
    }


    for (int i = 0; i <= N; ++i) {
        free(dp[i]);
    }
    free(dp);
    free(items);
    free(selected);

    return 0;
}