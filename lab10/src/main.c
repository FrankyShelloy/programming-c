#include "grehem.h"

int main() {
    int point_count;
    if (scanf("%d", &point_count) != 1) {
        printf("bad number of lines\n");
        return 0;
    }

    if (point_count < 0 || point_count > 100000) {
        printf("bad number of points\n");
        return 0;
    }

    Point *points = (Point *)malloc(point_count * sizeof(Point));
    if (!points) {
        printf("memory error\n");
        return 0;
    }

    for (int i = 0; i < point_count; i++) {
        if (scanf("%d %d", &points[i].x, &points[i].y) != 2) {
            printf("bad number of lines\n");
            free(points);
            return 0;
        }
    }

    if (point_count == 0) {
        free(points);
        return 0;
    }

    Point *convex_hull = (Point *)malloc((point_count + 1) * sizeof(Point));
    if (!convex_hull) {
        printf("memory error\n");
        free(points);
        return 0;
    }

    int hull_size = build_convex_hull(points, point_count, convex_hull);

    for (int i = 0; i < hull_size; i++) {
        printf("%d %d\n", convex_hull[i].x, convex_hull[i].y);
    }

    free(points);
    free(convex_hull);
    return 0;
}