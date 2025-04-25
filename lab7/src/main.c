#include <stdio.h>
#include <string.h>

#define MAX_N 2000
#define BITSET_SIZE ((MAX_N + 7) / 8)

#define WHITE 0
#define GRAY  1
#define BLACK 2

typedef struct {
    unsigned char adj[BITSET_SIZE];
} Graph;

Graph DAG[MAX_N];
int color[MAX_N];
int sorted[MAX_N];
int sortedSize = 0;
int hasCycle = 0;

void setBit(Graph* Graph, int pos) {
    Graph->adj[pos / 8] |= (1 << (pos % 8));
}

int isAdj(Graph* Graph, int pos) {
    return (Graph->adj[pos / 8] >> (pos % 8)) & 1;
}

void push(int Graph) {
    sorted[sortedSize++] = Graph + 1;
}

void dfs(int Graph) {
    if (color[Graph] == GRAY) {
        hasCycle = 1;
        return;
    }
    if (color[Graph] == BLACK) {
        return;
    }

    color[Graph] = GRAY;

    for (int each = 0; each < MAX_N; each++) {
        if (isAdj(&DAG[Graph], each)) {
            dfs(each);
        }
    }

    color[Graph] = BLACK;
    push(Graph);
}

void topSort(int N) {
    for (int each = 0; each < N; each++) {
        color[each] = WHITE;
    }

    for (int each = 0; each < N; each++) {
        if (color[each] == WHITE) {
            dfs(each);
        }
    }

    if (hasCycle) {
        printf("impossible to sort\n");
    } else {
        for (int each = sortedSize - 1; each >= 0; each--) {
            printf("%d ", sorted[each]);
        }
        printf("\n");
    }
}

int main() {
    int N, M;

    if (scanf("%d", &N) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (N < 0 || N > MAX_N) {
        printf("bad number of vertices\n");
        return 0;
    }

    if (scanf("%d", &M) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (M < 0 || (N > 0 && M > N * (N - 1) / 2)) {
        printf("bad number of edges\n");
        return 0;
    }

    for (int each = 0; each < N; each++) {
        memset(DAG[each].adj, 0, BITSET_SIZE);
    }

    for (int each = 0; each < M; each++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("bad number of lines\n");
            return 0;
        }
        if (u < 1 || u > N || v < 1 || v > N) {
            printf("bad vertex\n");
            return 0;
        }
        setBit(&DAG[u - 1], v - 1);
    }

    topSort(N);

    return 0;
}