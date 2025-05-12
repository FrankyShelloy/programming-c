#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define ULL_MAX ((unsigned long long)ULLONG_MAX)
#define NO_EDGE UINT_MAX
#define BUF_SIZE (1 << 20)

bool readInt(int *res);
bool readInput(int *n, int *s, int *f, int *m);
unsigned int* makeAdj(int n, int m);
void runDijk(int n, int s, unsigned int *adj, unsigned long long *dist, int *par, int *paths);
void printDist(int n, unsigned long long *dist);
void printPath(int n, int s, int f, unsigned long long *dist, int *par, int *paths);