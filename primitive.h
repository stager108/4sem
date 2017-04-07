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

    virtual lld m() = 0;
    virtual lld c() = 0;
    virtual lld k() = 0;
    virtual Point getNormal(Point T, Point S) = 0;
    virtual std::vector<ColouredPoint> getPointOfIntersecting(const Point a, const Point b) = 0;

 protected:
    bool isPointInsideTriangle(const Point A, const Point B, const Point C, const Point T) {
        return (areCoDirected((T - A)^(B - A) , (T - B)^(C - B)) && \
            areCoDirected((T - B)^(C - B), (T - C)^(A - C)));
    }

    std::vector<Point> getPointOfIntersectingLineAndPlane(const Point A, const Point B, const Point C,
                                                          const Point P1, const Point P2) {
        std::vector<Point> ans;
        lld w1 = (B - P1) * ((A - B) ^ (C - B));
        lld w2 = (B - P2) * ((A - B) ^ (C - B));
        if(fabsl(w1 - w2) > 0) {
            lld t = w1/(w1 - w2);
            if (t >= 0) {
                ans.resize(1);
                ans[0] = P1 + t * (P2 - P1);
            }
        }
        return ans;
    }
};

class Triangle : public Object {
 private:
    Point A, B, C;
    Colour colour;
    Colour back_colour;
    int normal;
    Material material;
 public:
    lld m(){
        return material.mirror;
    }
    lld c(){
        return material.clearance;
    }
    lld k(){
        return material.k;
    }
    Triangle(const Point a, const Point b, const Point c, const Colour colour, const Colour back, const Material m, const int n) :
            A(a), B(b), C(c), colour(colour), back_colour(back), material(m), normal(n) {}

    bool areIntersected(const Point P1, const Point P2) {
        std::vector<Point> p = getPointOfIntersectingLineAndPlane(A, B, C, P1, P2);
        return ((!areComplanar(B - C, A - C, P2 - P1)) && (p.size() > 0) &&
            isPointInsideTriangle(A, B, C, p[0]));
    }

    std::vector<ColouredPoint> getPointOfIntersecting(const Point P1, const Point P2) {
        std::vector<ColouredPoint> ans;
        if (areIntersected(P1, P2)) {
            ans.resize(1);
            std::vector<Point> p = getPointOfIntersectingLineAndPlane(A, B, C, P1, P2);
            if( ((P2 - P1)*((B - A)^(B - C)))*normal > 0) {
                ans[0] = ColouredPoint(p[0], colour);
            }
            else{
                ans[0] = ColouredPoint(p[0], back_colour);
            }
        }
        return ans;
    }
    Point getNormal(Point T, Point S){
        Point R = ((B - A)^(C - A));
        Point ans;
        if(R*(S - T) > 0){
            ans = R;
        }
        else{
            ans = (-1)*R;
        }
        return ans;
    }
};

class Sphere : public Object {
 private:
    lld R;
    Point O;
    Colour colour;
    Material material;
 public:
    Sphere(const lld r, const Point o, const Colour colour, const Material m)
            : R(r), O(o), colour(colour), material(m) {}

    lld m(){
        return material.mirror;
    }
    lld c(){
        return material.clearance;
    }
    lld k(){
        return material.k;
    }

    std::vector<ColouredPoint> getPointOfIntersecting(Point A, Point B) {
        lld a = (B - A).len2();
        lld b = 2 * ((A - O).x * (B - A).x + (A - O).y * (B - A).y + (A - O).z * (B - A).z);
        lld c = (A-O).len2()- R * R;
        lld D = b * b - 4 * a * c;

        std::vector<ColouredPoint> ans;
        if(D >= 0) {
            lld x1 = (-b + sqrtl(D)) / (2 * a);
            lld x2 = (-b - sqrtl(D)) / (2 * a);
            if(x1>=0){
                ans.push_back(ColouredPoint(A + x1 * (B-A), colour));
            }
            if(x2>=0){
                ans.push_back(ColouredPoint(A + x2 * (B-A), colour));
            }
        }
        return ans;
    }
    Point getNormal(Point T, Point S){
        Point R = (T - O);
        Point ans;
        lld N = R * (S - T);
        if(N > 0){
            ans = R;
        }
        else{
            if((N < 0)||(areCoDirected(R, S - T))){
                ans = (-1)*R;
            }
            else{
                 ans = (-1)*R;
            }
        }
        return ans;
    }

 private:

    bool areIntersected(Point P1, Point P2) {
        return isMoreThen(R, triangleHeightFromC(P1, P2, O));
    }
    lld triangleHeightFromC(const Point A, const Point B, const Point C) {
        return ((C - A) ^ (C - B)).len();
    }
};

class Quadrilateral : public Object {
 private:
    Point A, B, C, D;
    Colour colour;
    Material material;
    Colour back_colour;
    int normal;
 public:
    Quadrilateral(const Point a, const Point b, const Point c, const Point d, const Colour colour, const Colour back, const Material m, const int n) :
        A(a), B(b), C(c), D(d), colour(colour), material(m), back_colour(back), normal(n) {}

    lld m(){
        return material.mirror;
    }
    lld c(){
        return material.clearance;
    }
    lld k(){
        return material.k;
    }

    std::vector<ColouredPoint> getPointOfIntersecting(const Point P1, const Point P2) {
        std::vector<ColouredPoint> ans;
        if(areIntersected(P1,P2)) {
            ans.resize(1);
            if (areIntersecteds(A, B, C, P1, P2)) {
                std::vector<Point> p = getPointOfIntersectingLineAndPlane(A, B, C, P1, P2);
                if( ((P2 - P1)*((B - A)^(B - C)))*normal > 0) {
                    ans[0] = ColouredPoint(p[0], colour);
                }
                else{
                    ans[0] = ColouredPoint(p[0], back_colour);
                }
            } else {
                std::vector<Point> p = getPointOfIntersectingLineAndPlane(D, B, C, P1, P2);
                if( ((P2 - P1)*((B - A)^(B - C)))*normal > 0) {
                    ans[0] = ColouredPoint(p[0], colour);
                }
                else{
                    ans[0] = ColouredPoint(p[0], back_colour);
                }
            }
        }
        return ans;
    }

    Point getNormal(Point T, Point S){
        Point R = ((B - A)^(C - A));
        Point ans;
        if(R*(S - T) >= 0){
            ans = R;
        }
        else{
            ans = (-1)*R;
        }
        return ans;
    }

 private:

    bool areIntersected(const Point P1, const Point P2) {
        return areIntersecteds(A, B, C, P1, P2) || areIntersecteds(A, C, D, P1, P2);
    }

    bool areIntersecteds(const Point a, const Point b, const Point c, const Point P1, const Point P2) {
        std::vector<Point> p = getPointOfIntersectingLineAndPlane(a, b, c, P1, P2);
        return ((!areComplanar(b - c, a - c, P2 - P1)) && (p.size() > 0) &&
            isPointInsideTriangle(a, b, c, p[0]));
    }

};
