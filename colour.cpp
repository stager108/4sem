#include "colour.h"

Colour operator+(const Colour c1, const Colour c2) {
	return Colour( std::min(c1.r + c2.r, MAX_RGB),
				   std::min(c1.g + c2.g, MAX_RGB),
				   std::min(c1.b + c2.b, MAX_RGB));
}

Colour operator*(const Colour c1, const Colour c2) {
	return Colour( std::min(c1.r * c2.r, MAX_RGB),
				   std::min(c1.g * c2.g, MAX_RGB),
				   std::min(c1.b * c2.b, MAX_RGB));
}

Colour operator*(const Colour c, const int k) {
	return Colour( std::min(c.r * k, MAX_RGB),
				   std::min(c.g * k, MAX_RGB),
				   std::min(c.b * k, MAX_RGB));
}

Colour operator*(const int k, const Colour c) {
	return Colour(std::min(c.r * k, MAX_RGB),
				  std::min(c.g * k, MAX_RGB),
				  std::min(c.b * k, MAX_RGB));
}
