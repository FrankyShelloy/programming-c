#include "grehem.h"
#include <stdint.h>

int comparePoints(const void *a, const void *b) {
    const Point *pointA = (const Point *)a;
    const Point *pointB = (const Point *)b;

    if (pointA->x < pointB->x) {
        return -1;
    }
    if (pointA->x > pointB->x) {
        return 1;
    }
    if (pointA->y < pointB->y) {
        return -1;
    }
    if (pointA->y > pointB->y) {
        return 1;
    }
    return 0;
}

int crossProduct(const Point *origin, const Point *a, const Point *b) {
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

int buildConvexHull(Point *points, int pointCount, Point *hull) {
    if (pointCount <= 1) {
        for (int i = 0; i < pointCount; i++) {
            hull[i] = points[i];
        }
        return pointCount;
    }

    qsort(points, pointCount, sizeof(Point), comparePoints);

    int hullSize = 0;

    for (int i = 0; i < pointCount; i++) {
        while (hullSize >= 2 && crossProduct(&hull[hullSize - 2], &hull[hullSize - 1], &points[i]) <= 0) {
            hullSize--;
        }
        hull[hullSize++] = points[i];
    }

    for (int i = pointCount - 2, minHullSize = hullSize + 1; i >= 0; i--) {
        while (hullSize >= minHullSize && crossProduct(&hull[hullSize - 2], &hull[hullSize - 1], &points[i]) <= 0) {
            hullSize--;
        }
        hull[hullSize++] = points[i];
    }

    return hullSize - 1;
}