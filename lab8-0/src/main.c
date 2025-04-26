#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int u, v, weight;
} Edge;

int compareEdges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeA->weight - edgeB->weight;
}

int findParent(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = findParent(parent, parent[i]);
}

void unionSets(int parent[], int rank[], int x, int y) {
    int xroot = findParent(parent, x);
    int yroot = findParent(parent, y);

    if (rank[xroot] < rank[yroot])
        parent[xroot] = yroot;
    else if (rank[xroot] > rank[yroot])
        parent[yroot] = xroot;
    else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

int main() {
    int N, M;
    if (scanf("%d", &N) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (N < 0 || N > 5000) {
        printf("bad number of vertices\n");
        return 0;
    }

    if (scanf("%d", &M) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (M < 0 || M > N * (N - 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }

    Edge* edges = malloc(M * sizeof(Edge));
    if (!edges) {
        return 0;
    }

    for (int i = 0; i < M; i++) {
        int u, v, weight;
        if (scanf("%d %d %d", &u, &v, &weight) != 3) {
            printf("bad number of lines\n");
            free(edges);
            return 0;
        }
        if (u < 1 || u > N || v < 1 || v > N) {
            printf("bad vertex\n");
            free(edges);
            return 0;
        }
        if (weight < 0 || weight > INT_MAX) {
            printf("bad length\n");
            free(edges);
            return 0;
        }
        edges[i].u = u - 1;
        edges[i].v = v - 1;
        edges[i].weight = weight;
    }

    if (N == 0 || (N > 1 && M < N - 1)) {
        printf("no spanning tree\n");
        free(edges);
        return 0;
    }

    qsort(edges, M, sizeof(Edge), compareEdges);

    int* parent = malloc(N * sizeof(int));
    int* rank = malloc(N * sizeof(int));
    if (!parent || !rank) {
        free(edges);
        free(parent);
        free(rank);
        return 0;
    }

    for (int i = 0; i < N; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    Edge* result = malloc((N - 1) * sizeof(Edge));
    if (!result) {
        free(edges);
        free(parent);
        free(rank);
        return 0;
    }
    int e = 0;
    int i = 0;

    while (e < N - 1 && i < M) {
        Edge next_edge = edges[i++];

        int x = findParent(parent, next_edge.u);
        int y = findParent(parent, next_edge.v);

        if (x != y) {
            result[e++] = next_edge;
            unionSets(parent, rank, x, y);
        }
    }

    if (e != N - 1) {
        printf("no spanning tree\n");
    } else {
        for (int i = 0; i < e; i++) {
            printf("%d %d\n", result[i].u + 1, result[i].v + 1);
        }
    }

    free(edges);
    free(parent);
    free(rank);
    free(result);
    return 0;
}