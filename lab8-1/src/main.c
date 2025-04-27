#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 5000

int findMinKeyVertex(unsigned int *keyValues, char *visitedVertices, int vertexCount) {
    unsigned int minValue = UINT_MAX;
    int minIndex = -1;

    for (int vertex = 0; vertex < vertexCount; vertex++) {
        if (!visitedVertices[vertex] && keyValues[vertex] < minValue) {
            minValue = keyValues[vertex];
            minIndex = vertex;
        }
    }
    return minIndex;
}

int main() {
    int vertexCount, edgeCount;

    // Read input and validate
    if (scanf("%d %d", &vertexCount, &edgeCount) != 2) {
        printf("bad number of lines\n");
        return 0;
    }

    if (vertexCount < 0 || vertexCount > MAX_VERTICES) {
        printf("bad number of vertices\n");
        return 0;
    }

    if (edgeCount < 0 || edgeCount > vertexCount * (vertexCount - 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }

    if (vertexCount == 0) {
        printf("no spanning tree\n");
        return 0;
    }

    unsigned int **adjacencyMatrix = (unsigned int **)malloc(vertexCount * sizeof(unsigned int *));
    for (int i = 0; i < vertexCount; i++) {
        adjacencyMatrix[i] = (unsigned int *)malloc(vertexCount * sizeof(unsigned int));
        for (int j = 0; j < vertexCount; j++) {
            adjacencyMatrix[i][j] = UINT_MAX;
        }
    }

    for (int i = 0; i < edgeCount; i++) {
        int fromVertex, toVertex, edgeLength;
        if (scanf("%d %d %d", &fromVertex, &toVertex, &edgeLength) != 3) {
            printf("bad number of lines\n");
            for (int j = 0; j < vertexCount; j++) free(adjacencyMatrix[j]);
            free(adjacencyMatrix);
            return 0;
        }

        if (fromVertex < 1 || fromVertex > vertexCount || toVertex < 1 || toVertex > vertexCount) {
            printf("bad vertex\n");
            for (int j = 0; j < vertexCount; j++) free(adjacencyMatrix[j]);
            free(adjacencyMatrix);
            return 0;
        }

        if (edgeLength < 0 || edgeLength > INT_MAX) {
            printf("bad length\n");
            for (int j = 0; j < vertexCount; j++) free(adjacencyMatrix[j]);
            free(adjacencyMatrix);
            return 0;
        }

        fromVertex--; toVertex--;
        adjacencyMatrix[fromVertex][toVertex] = edgeLength;
        adjacencyMatrix[toVertex][fromVertex] = edgeLength;
    }

    char visitedVertices[MAX_VERTICES] = {0};
    unsigned int keyValues[MAX_VERTICES];
    int parentVertices[MAX_VERTICES];

    for (int i = 0; i < vertexCount; i++) {
        keyValues[i] = UINT_MAX;
        parentVertices[i] = -1;
    }
    keyValues[0] = 0;

    for (int count = 0; count < vertexCount; count++) {
        int currentVertex = findMinKeyVertex(keyValues, visitedVertices, vertexCount);

        if (currentVertex == -1) {
            printf("no spanning tree\n");
            for (int j = 0; j < vertexCount; j++) free(adjacencyMatrix[j]);
            free(adjacencyMatrix);
            return 0;
        }

        visitedVertices[currentVertex] = 1;

        for (int neighbor = 0; neighbor < vertexCount; neighbor++) {
            if (adjacencyMatrix[currentVertex][neighbor] != UINT_MAX &&
                !visitedVertices[neighbor] &&
                adjacencyMatrix[currentVertex][neighbor] < keyValues[neighbor]) {
                parentVertices[neighbor] = currentVertex;
                keyValues[neighbor] = adjacencyMatrix[currentVertex][neighbor];
            }
        }
    }

    for (int i = 0; i < vertexCount; i++) {
        if (!visitedVertices[i]) {
            printf("no spanning tree\n");
            for (int j = 0; j < vertexCount; j++) free(adjacencyMatrix[j]);
            free(adjacencyMatrix);
            return 0;
        }
    }

    for (int i = 1; i < vertexCount; i++) {
        if (parentVertices[i] != -1) {
            printf("%d %d\n", parentVertices[i] + 1, i + 1);
        }
    }

    for (int i = 0; i < vertexCount; i++) free(adjacencyMatrix[i]);
    free(adjacencyMatrix);

    return 0;
}