#ifndef GREHEM_H
#define GREHEM_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

int compare_points(const void *a, const void *b);
int cross_product(const Point *origin, const Point *a, const Point *b);
int build_convex_hull(Point *points, int point_count, Point *hull);

#endif