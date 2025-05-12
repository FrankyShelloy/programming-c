#include "dijkstra.h"

void runDijk(int n, int s, unsigned int *adj, unsigned long long *dist, int *par, int *paths) {
    bool *vis = calloc((size_t)n, sizeof(bool));
    for (int i = 0; i < n; i++) {
        dist[i] = ULL_MAX; par[i] = -1; paths[i] = 0;
    }
    dist[s-1] = 0; paths[s-1] = 1;
    for (int it = 0; it < n; it++) {
        int u = -1;
        unsigned long long min_d = ULL_MAX;
        for (int i = 0; i < n; i++)
            if (!vis[i] && dist[i] < min_d) {
                min_d = dist[i]; u = i;
            }
        if (u < 0 || min_d == ULL_MAX) break;
        vis[u] = true;
        size_t base = (size_t)u * n;
        for (int v = 0; v < n; v++) {
            if (vis[v]) continue;
            unsigned int w = adj[base + v];
            if (w == NO_EDGE) continue;
            unsigned long long new_d = min_d + w;
            if (new_d < dist[v]) {
                dist[v] = new_d;
                par[v] = u;
                paths[v] = paths[u];
            }
            else if (new_d == dist[v] && paths[v] < 2) {
                paths[v] += paths[u];
                if (paths[v] > 2) paths[v] = 2;
            }
        }
    }
    free(vis);
}