#include "dijkstra.h"

static char buf[BUF_SIZE];
static int buf_pos = 0, buf_len = 0;

static inline char getChar() {
    if (buf_pos >= buf_len) {
        buf_len = fread(buf, 1, BUF_SIZE, stdin);
        buf_pos = 0;
        if (!buf_len) return EOF;
    }
    return buf[buf_pos++];
}

bool readInt(int *res) {
    char c;
    int sign = 1, val = 0;
    do {
        c = getChar();
        if (c == EOF) return false;
    } while (c != '-' && (c < '0' || c > '9'));
    if (c == '-') {
        sign = -1;
        c = getChar();
    }
    for (; c >= '0' && c <= '9'; c = getChar())
        val = val * 10 + (c - '0');
    *res = val * sign;
    return true;
}

bool readInput(int *n, int *s, int *f, int *m) {
    if (!readInt(n)) return false;
    if (*n < 0 || *n > 5000) {
        printf("bad number of vertices\n"); return false;
    }
    if (!readInt(s) || !readInt(f)) {
        printf("bad number of lines\n"); return false;
    }
    if (*s < 1 || *s > *n || *f < 1 || *f > *n) {
        printf("bad vertex\n"); return false;
    }
    if (!readInt(m)) {
        printf("bad number of lines\n"); return false;
    }
    long long max_e = (long long)*n * (*n - 1LL) / 2;
    if (*m < 0 || *m > max_e) {
        printf("bad number of edges\n"); return false;
    }
    return true;
}

unsigned int* makeAdj(int n, int m) {
    size_t size = (size_t)n * n;
    unsigned int *adj = malloc(size * sizeof(unsigned int));
    if (!adj) return NULL;
    memset(adj, 0xFF, size * sizeof(unsigned int));
    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (!readInt(&u) || !readInt(&v) || !readInt(&w)) {
            printf("bad number of lines\n"); exit(0);
        }
        if (u < 1 || u > n || v < 1 || v > n) {
            printf("bad vertex\n"); exit(0);
        }
        if (w < 0 || w > INT_MAX) {
            printf("bad length\n"); exit(0);
        }
        size_t idx1 = (size_t)(u-1) * n + (v-1);
        size_t idx2 = (size_t)(v-1) * n + (u-1);
        unsigned int uw = (unsigned int)w;
        if (uw < adj[idx1]) {
            adj[idx1] = uw;
            adj[idx2] = uw;
        }
    }
    return adj;
}

void printDist(int n, unsigned long long *dist) {
    for (int i = 0; i < n; i++) {
        if (dist[i] == ULL_MAX) printf("oo");
        else if (dist[i] > INT_MAX) printf("INT_MAX+");
        else printf("%llu", dist[i]);
        if (i + 1 < n) printf(" ");
    }
    printf("\n");
}

void printPath(int n, int s, int f, unsigned long long *dist, int *par, int *paths) {
    if (dist[f-1] == ULL_MAX) {
        printf("no path\n"); return;
    }
    if (dist[f-1] > INT_MAX && paths[f-1] > 1) {
        printf("overflow\n"); return;
    }
    int cur = f - 1;
    bool first = true;
    while (cur != -1) {
        if (!first) printf(" ");
        printf("%d", cur + 1);
        first = false;
        cur = par[cur];
    }
    printf("\n");
}

int main() {
    int n, s, f, m;
    if (!readInput(&n, &s, &f, &m)) return 0;
    unsigned int *adj = makeAdj(n, m);
    if (!adj) return 0;
    unsigned long long *dist = malloc((size_t)n * sizeof(unsigned long long));
    int *par = malloc((size_t)n * sizeof(int));
    int *paths = malloc((size_t)n * sizeof(int));
    runDijk(n, s, adj, dist, par, paths);
    printDist(n, dist);
    printPath(n, s, f, dist, par, paths);
    free(adj); free(dist); free(par); free(paths);
    return 0;
}