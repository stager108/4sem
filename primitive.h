#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>
#include <stack>
#include <cmath>

#include "structures.h"
class Object {
public:
	virtual ~Object() {}
	virtual bool areIntersected(Point a, Point b) = 0;
	virtual std::vector<ColouredPoint> intersectedInPoint(const Point a, const Point b) = 0;

protected:
	bool isPointInsideTriangle(const Point A, const Point B, const Point C, const Point T){
		return (areCoDirected((B - A)^(T - A),(C - B)^(T - B)) && \
            areCoDirected((C - B)^(T - B), (A - C)^(T - C)));
	}

	Point getPointOfIntersectingLineAndPlane(const Point A, const Point B, const Point C,
											 const Point P1, const Point P2) {
		Point Ans;
		lld w1 = (P1 - B) * ((B - A) ^ (B - C));
		lld w2 = (P2 - B) * ((B - A) ^ (B - C));
		if (isZero(w1)) {
			Ans = P1;
		} else {
			if (isZero(w2)) {
				Ans = P2;
			} else {
				if (sign(w1) * sign(w2) < 0) {
					Ans = P1 + (P2 - P1) * fabsl(w1 / (w1 - w2));
				} else {
					if (fabsl(w1) > fabsl(w2)) {
						Ans = P2 + (P1 - P2) * fabsl((w1 - w2) / w2);
					} else {
						Ans = P1 + (P2 - P1) * fabsl((w1 - w2) / w1);
					}
				}
			}
		}
		return Ans;
	}
};

class Triangle : public Object {
private:
    Point A, B, C;
	Colour colour;
	Material matherial;
public:
    Triangle(const Point a, const Point b, const Point c, const Colour colour):A(a),B(b),C(c),colour(colour){}

    bool areIntersected(const Point P1, const Point P2){
        return isPointInsideTriangle(A,B,C, getPointOfIntersectingLineAndPlane(A, B, C, P1, P2));
    }

    std::vector<ColouredPoint> getPointOfIntersecting(const Point P1, const Point P2){
        std::vector<ColouredPoint> ans(1);
        ans[0] = ColouredPoint(getPointOfIntersectingLineAndPlane(A, B, C, P1, P2), colour);
        return ans;
    }
};

class Sphere : public Object {
private:
    lld R;
    Point O;
	Colour colour;
	Material matherial;
public:
    Sphere(const lld r, const Point o, const Colour colour):R(r),O(o),colour(colour){}

    bool areIntersected(Point P1, Point P2){
        return isMoreThen(R, triangleHeightFromC(P1, P2, O));
    }

	std::vector<ColouredPoint> getPointOfIntersecting(Point A, Point B){
        lld a = (B - A).len2();
        lld b = 2 * ((B - A).x + (B - A).y + (B - A).z);
        lld c = A.len2() + O.len2() - R * R;
        lld D = b * b - 4 * a * c;
        lld x1 = (- b + sqrtl(D)) / (2 * a);
        lld x2 = (- b - sqrtl(D)) / (2 * a);
		std::vector<ColouredPoint> ans(2);
        ans[0] = ColouredPoint(A + x1 * (B - A), colour);
        ans[1] = ColouredPoint(A + x2 * (B - A), colour);
        return ans;
    }
private:
	lld triangleHeightFromC(const Point A, const Point B, const Point C){
		return ((C - A) ^ (C - B)).len() / (B - A).len();
	}
};

class Quadrilateral : public Object {
private:
    Point A, B, C, D;
	Colour colour;
	Material matherial;
public:
    Quadrilateral(const Point a, const Point b, const Point c, const Point d, const Colour colour):
			A(a),B(b),C(c),D(d),colour(colour){}

    bool areIntersected(const Point P1, const Point P2){
        return isLineIntersectedTriangle(A, B, C, P1, P2) || isLineIntersectedTriangle(B, C, D, P1, P2);
    }

	std::vector<ColouredPoint> getPointOfIntersecting(const Point P1, const Point P2){
		std::vector<ColouredPoint> ans(1);
        if(isLineIntersectedTriangle(A,B,C,P1,P2)){
            ans[0] = ColouredPoint(getPointOfIntersectingLineAndPlane(A, B, C, P1, P2), colour);
        }
        else{
            ans[0] = ColouredPoint(getPointOfIntersectingLineAndPlane(D, B, C, P1, P2), colour);
        }
        return ans;
    }

private:
	bool isLineIntersectedTriangle(const Point A, const Point B, const Point C, const Point P1, const Point P2){
		// obrabotat sluchai prohogdeniya pryamoi v toi zhe ploskosti, chto i treugolnik!
		bool ans = false;
		if ( !areComplanar(B - A, B - C, P1 - P2)){
			Point T = getPointOfIntersectingLineAndPlane(A,B,C,P1,P2);
			if (isPointInsideTriangle(A,B,C,T))
				ans = true;
		}
		return ans;
	}
};
