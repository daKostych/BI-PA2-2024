#ifndef __TRAINER__
#include <cmath>
#include <cassert>

#define EPSILON 1e-9

bool dblEqual(double a, double b)
{
    return std::fabs(a - b) < EPSILON;
}

struct Point2D
{
    double x, y;
};

struct Point3D
{
    double x, y, z;
};

// ax+by+c=0
struct Line
{
    double a, b, c;
};

// ax+by+cz+d=0
struct Plane
{
    double a, b, c, d;
};

#endif

double dist(const Point2D &point1, const Point2D &point2 = Point2D{0, 0})
{
    return std::sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y));
}

double dist(const Point3D &point1, const Point3D &point2 = Point3D{0, 0, 0})
{
    return std::sqrt((point1.x - point2.x) * (point1.x - point2.x) + (point1.y - point2.y) * (point1.y - point2.y) + (point1.z - point2.z) * (point1.z - point2.z));
}

double dist(const Line &line, const Point2D &point = Point2D{0, 0})
{
    double numerator = std::fabs(line.a * point.x + line.b * point.y + line.c);
    double denominator = std::sqrt(line.a * line.a + line.b * line.b);

    return numerator / denominator;
}

double dist(const Plane &plane, const Point3D &point = Point3D{0, 0, 0})
{
    double numerator = std::fabs(plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d);
    double denominator = std::sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);

    return numerator / denominator;
}

#ifndef __TRAINER__
int main()
{
    assert(dblEqual(dist(Point2D{1, 1}, Point2D{2, 2}), std::sqrt(2)));
    assert(!dblEqual(dist(Point2D{1, 1}, Point2D{2, 2}), 1.4));
    assert(dblEqual(dist(Point2D{21, 23}, Point2D{53, 11}), 34.17601498127));
    assert(dblEqual(dist(Point2D{1234, 321}), 1275.0674491963));
    assert(dblEqual(dist(Point3D{123, 456, 789}, Point3D{666, 42, 2048}), 1432.2450907579));
    assert(dblEqual(dist(Point3D{37, 26, 15}), 47.644516998286));
    assert(dblEqual(dist(Line{2, 1, 5}, Point2D{3, 7}), 8.049844719));
    assert(!dblEqual(dist(Line{2, 1, 5}, Point2D{3, 7}), 8));
    assert(dblEqual(dist(Line{3, 5, 7}), 1.200490096));
    assert(!dblEqual(dist(Line{3, 5, 7}), 1.2));
    assert(dblEqual(dist(Plane{13, 7, 29, 17}, Point3D{11, 53, 47}), 58.20120744182629));
    assert(!dblEqual(dist(Plane{13, 7, 29, 17}, Point3D{11, 53, 47}), 58.2));
    assert(dblEqual(dist(Plane{2, 4, 8, 16}), 1.745743121887939));
}
#endif