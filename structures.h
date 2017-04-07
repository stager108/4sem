#pragma once

#include "geom_utils.h"

struct ColouredPoint {
    Point point;
    Colour colour;

    ColouredPoint() : point(0, 0, 0), colour(0, 0, 0) {}
    ColouredPoint(Point point, Colour colour) : point(point), colour(colour) {}
};

struct LightPoint {
    // Absolute location
    Point location;
    lld k;
    LightPoint():location(0,0,0), k(0){}

    LightPoint(Point l, lld k):location(l), k(k){}
    //Colour colour;
};

struct Material {
    // Required:
    // Ka + Kd + Ks + Kr + Kt = 1.0
    lld mirror;  //zerkalnost
    lld clearance; //prozrachnost
    lld k;
    Material():mirror(0), clearance(0){}
    Material(lld m, lld c, lld k): mirror(m/100),clearance(c/100), k(k){}

};
