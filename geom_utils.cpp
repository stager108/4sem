#include "geom_utils.h"

bool areEqual(lld x, lld y) {
	return sign(x - y) == 0;
}

bool isMoreThen(lld x, lld y) {
	return sign(x - y) == 1;
}

bool isLessThen(lld x, lld y) {
	return sign(x - y) == -1;
}

bool isZero(Point P) {
	return isZero(P.x) && isZero(P.y) && isZero(P.z);
}

Point operator+(const Point p1, const Point p2) {
	return Point(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

Point operator-(const Point p1, const Point p2) {
	return Point(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

Point operator*(lld d, const Point p) {
	return Point(d * p.x, d * p.y, d * p.z);
}

Point operator*(const Point p, lld d) {
	return d*p;
}

Point operator/(const Point p, lld d) {
	return Point( p.x / d , p.y / d , p.z / d);
}

lld operator*(const Point p1, const Point p2) {
	return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

Point operator^ (const Point p1, const Point p2) {
   // std::cout <<"^^ "<< p1.y * p2.z - p1.z * p2.y << " " << p1.z * p2.x - p1.x * p2.z<< " " <<p1.x * p2.y - p1.y * p2.x<<std::endl;
	return Point(p1.y * p2.z - p1.z * p2.y , p1.z * p2.x - p1.x * p2.z , p1.x * p2.y - p1.y * p2.x);
}

bool areEqual(const Point p1, const Point p2) {
	return areEqual(p1.x, p2.x) && areEqual(p1.y, p2.y) && areEqual(p1.z, p2.z);
}

bool areCollinear(const Point p1, const Point p2) {
	return sign( p1 * p2) == 0;
}

bool isOnLine(const Point p, const Point a, const Point b) {
	return areCollinear( p - a, b - a);
}

bool areLinesParallelOrEqual(const Point A1, const Point A2, const Point B1, const Point B2) {
	return areCollinear(A2 - A1, B2 - B1);
}

bool areComplanar(const Point A1, const Point A2, const Point B1, const Point B2, const Point C1, const Point C2) {
	return isZero((A2 - A1)*((B2 - B1)^(C2 - C1)));
}

bool areComplanar(const Point V1, const Point V2, const Point V3) {
	return isZero(V1*(V2^V3));
}

bool areCoDirected(Point A, Point B) {
   // std::cout << " * "<< (A * B) << " " << (A ^ B).len()<< std::endl;
	return isZero(A ^ B) && (sign(A * B) >= 0);
}

bool areOppositeDirected(Point A, Point B) {
	return isZero(A ^ B) && (sign(A * B) <= 0);
}

bool isZero(lld z) {
	return(fabsl(z)<eps);
}

int sign(lld z) {
	if(isZero(z))
		return 0;
	return (z>0)?1:-1;
}

Point mirrorRay(Point ray, Point normal){
	//std::cout <<"ray "<< ray.x << " "<<ray.y << " "<<ray.z<<std::endl;
	Point ans = ray - 2 * normal * (ray * normal /normal.len2());
	//std::cout <<"ans "<< ans.x << " "<<ans.y << " "<<ans.z<<std::endl;
	return ans;
}
Point brokenRay(Point ray, Point normal){
	Point w = normal * (ray * normal /normal.len2());
	return ray;
}