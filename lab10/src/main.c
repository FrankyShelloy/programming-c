#include "grehem.h"

int main() {
    int pointCount;
    if (scanf("%d", &pointCount) != 1) {
        printf("bad number of lines\n");
        return 0;
    }

    if (pointCount < 0 || pointCount > 100000) {
        printf("bad number of points\n");
        return 0;
    }

    Point *points = (Point *)malloc(pointCount * sizeof(Point));
    if (!points) {
        printf("memory error\n");
        return 0;
    }

    for (int i = 0; i < pointCount; i++) {
        if (scanf("%d %d", &points[i].x, &points[i].y) != 2) {
            printf("bad number of lines\n");
            free(points);
            return 0;
        }
    }

    if (pointCount == 0) {
        free(points);
        return 0;
    }

    Point *convexHull = (Point *)malloc((pointCount + 1) * sizeof(Point));
    if (!convexHull) {
        printf("memory error\n");
        free(points);
        return 0;
    }

    int hullSize = buildConvexHull(points, pointCount, convexHull);

    for (int i = 0; i < hullSize; i++) {
        printf("%d %d\n", convexHull[i].x, convexHull[i].y);
    }

    free(points);
    free(convexHull);
    return 0;
}