#include "func.h"

void knapsack(Item *items, int N, int W, int **dp, int *selected) {
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

    int w = W;
    for (int i = N; i > 0; --i) {
        if (dp[i][w] != dp[i-1][w]) {
            selected[i-1] = 1;
            w -= items[i-1].weight;
        }
    }
}

int get_total_value(int **dp, int N, int W) {
    return dp[N][W];
}
