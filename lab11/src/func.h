#ifndef FUNC_H
#define FUNC_H

typedef struct {
    int weight;
    int value;
    int index;
} Item;

void knapsack(Item *items, int N, int W, int **dp, int *selected);
int get_total_value(int **dp, int N, int W);

#endif
