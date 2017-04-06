#pragma once

#include "geom_utils.h"

struct ColouredPoint {
    Point point;
    Colour colour;

    ColouredPoint() : point(0, 0, 0), colour(0, 0, 0) {}
    ColouredPoint(Point point, Colour colour) : point(point), colour(colour) {}
};

struct LightSource {
    // Absolute location
    Point location_world;
    // Location on projection
    Point location;
    Colour colour;
};

struct Material {
    // Required:
    // Ka + Kd + Ks + Kr + Kt = 1.0
    lld mirror = 0;  //zerkalnost
    lld clearance = 0; //prozrachnost
    Material(lld m, lld c): mirror(m),clearance(c){}

};
