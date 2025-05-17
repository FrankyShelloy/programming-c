#include "grehem.h"
#include <stdint.h>

int compare_points(const void *a, const void *b) {
    const Point *point_a = (const Point *)a;
    const Point *point_b = (const Point *)b;

    if (point_a->x < point_b->x) {
        return -1;
    }
    if (point_a->x > point_b->x) {
        return 1;
    }
    if (point_a->y < point_b->y) {
        return -1;
    }
    if (point_a->y > point_b->y) {
        return 1;
    }
    return 0;
}

int cross_product(const Point *origin, const Point *a, const Point *b) {
    const double dx1 = (double)a->x - origin->x;
    const double dy1 = (double)a->y - origin->y;
    const double dx2 = (double)b->x - origin->x;
    const double dy2 = (double)b->y - origin->y;
    const double product = dx1 * dy2 - dx2 * dy1;

    if (product > 0) {
        return 1;
    }
    if (product < 0) {
        return -1;
    }
    return 0;
}

int build_convex_hull(Point *points, int point_count, Point *hull) {
    if (point_count <= 1) {
        for (int i = 0; i < point_count; i++) {
            hull[i] = points[i];
        }
        return point_count;
    }

    qsort(points, point_count, sizeof(Point), compare_points);

    int hull_size = 0;

    for (int i = 0; i < point_count; i++) {
        while (hull_size >= 2 && cross_product(&hull[hull_size - 2], &hull[hull_size - 1], &points[i]) <= 0) {
            hull_size--;
        }
        hull[hull_size++] = points[i];
    }

    for (int i = point_count - 2, min_hull_size = hull_size + 1; i >= 0; i--) {
        while (hull_size >= min_hull_size && cross_product(&hull[hull_size - 2], &hull[hull_size - 1], &points[i]) <= 0) {
            hull_size--;
        }
        hull[hull_size++] = points[i];
    }

    return hull_size - 1;
}