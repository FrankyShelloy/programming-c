#ifndef GREHEM_H
#define GREHEM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

int comparePoints(const void *a, const void *b);
int crossProduct(const Point *origin, const Point *a, const Point *b);
int buildConvexHull(Point *points, int pointCount, Point *hull);

#endif