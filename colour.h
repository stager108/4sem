#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <stack>
#include <cmath>

#include "geom_utils.h"

using lld =  long double;
const int MAX_RGB  = 255;

struct Colour{
    int r, g, b;
	Colour() :r(0), g(0), b(0) {}
	Colour(int r, int g, int b):g(g), r(r), b(b) {}
	Colour(const Colour& other) :r(other.r), g(other.g), b(other.b) {}

    Colour getGray(const Colour c) {
        const uint8_t gray = uint8_t(c.r * 0.2126 + c.g * 0.7152 + c.b * 0.0722);
        return Colour(gray, gray, gray);
    }
};

Colour operator+(const Colour c1, const Colour c2);

Colour operator * (const Colour c1, const Colour c2);

Colour operator * (const Colour c, const int k);

Colour operator * (const int k, const Colour c);

Colour operator*( const Colour c, const lld k);
Colour operator*( const lld k, const Colour c);
