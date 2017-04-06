#include "colour.h"

Colour operator +(const Colour c1, const Colour c2) {
    return Colour(std::min(c1.r + c2.r, MAX_RGB),
                  std::min(c1.g + c2.g, MAX_RGB),
                  std::min(c1.b + c2.b, MAX_RGB));
}

Colour operator *(const Colour c1, const Colour c2) {
    return Colour(std::min(c1.r * c2.r, MAX_RGB),
                  std::min(c1.g * c2.g, MAX_RGB),
                  std::min(c1.b * c2.b, MAX_RGB));
}

Colour operator*(const Colour c, const int k) {
    return Colour(std::min(c.r * k, MAX_RGB),
                  std::min(c.g * k, MAX_RGB),
                  std::min(c.b * k, MAX_RGB));
}

Colour operator*(const int k, const Colour c) {
    return Colour(std::min((c.r * k), MAX_RGB),
                  std::min((c.g * k), MAX_RGB),
                  std::min((c.b * k), MAX_RGB));
}

Colour operator*( const Colour c, const lld k) {
    lld x = c.r * k;
    lld y = c.g * k;
    lld z = c.b * k;
    int r = roundl(x);
    int g = roundl(y);
    int b = roundl(z);
    //std::cout << " x "<<x << " y" << y <<" z" << z <<std::endl;
    return Colour((int) std::min(r, MAX_RGB),
                  (int)std::min(g, MAX_RGB),
                  (int)std::min(b, MAX_RGB));
}

Colour operator*( const lld k, const Colour c) {
    lld x = c.r * k;
    lld y = c.g * k;
    lld z = c.b * k;
    int r = roundl(x);
    int g = roundl(y);
    int b = roundl(z);

   // std::cout << " x "<<x << " y" << y <<" z" << z <<std::endl;
    return Colour((int) std::min(r, MAX_RGB),
                  (int)std::min(g, MAX_RGB),
                  (int)std::min(b, MAX_RGB));
    /*
    return Colour((int)std::min((c.r * k), (lld)MAX_RGB),
                  (int)std::min((c.g * k), (lld)MAX_RGB),
                  (int)std::min((c.b * k),(lld) MAX_RGB));
                  */
}