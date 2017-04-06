#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <stack>
#include <cmath>

#include "colour.h"

using lld =  long double;

const long double eps = 1e-10;

bool isZero(lld z);

int sign(lld z);

bool areEqual(lld x, lld y);
bool isMoreThen(lld x, lld y);
bool isLessThen(lld x, lld y);

struct Point {
    lld x, y, z;

    lld len2() const {
        return x * x + y * y + z * z;
    }
    lld len() const {
        return sqrtl(x * x + y * y + z * z);
    }
    Point() : x(0), y(0), z(0) {}
    Point(lld x, lld y, lld z) : x(x), y(y), z(z) {}
    Point(const Point &other) : x(other.x), y(other.y), z(other.z) {}

};

bool isZero(Point P);

Point operator+(const Point p1, const Point p2);

Point operator-(const Point p1, const Point p2);

/*Point operator * (const Point p1, const Point p2){
    return Point(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z);
}*/

Point operator*(lld d, const Point p);

Point operator*(const Point p, lld d);

Point operator/( const Point p, lld d);

lld operator*(const Point p1, const Point p2);

Point operator^(const Point p1, const Point p2);

bool areEqual(const Point p1, const Point p2);

bool areCollinear(const Point p1, const Point p2);

bool isOnLine(const Point p, const Point a, const Point b);

bool areLinesParallelOrEqual(const Point A1, const Point A2, const Point B1, const Point B2);

bool areComplanar(const Point A1, const Point A2, const Point B1, const Point B2,
                  const Point C1, const Point C2);

bool areComplanar(const Point V1, const Point V2, const Point V3);

bool areCoDirected(Point A, Point B);

bool areOppositeDirected(Point A, Point B);

Point mirrorRay(Point ray, Point normal);
Point brokenRay(Point ray, Point normal);