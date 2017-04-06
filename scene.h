#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <stack>
#include <cmath>
#include "primitive.h"

class Scene {
 private:
    std::vector<std::shared_ptr<Object>> Figures;
    std::vector<Point> Lights;
    ColouredPoint getFoton(ColouredPoint source, Point A) {
        Point ans = source.point + (A - source.point)/((A - source.point).len()) *eps;
        lld lightness = 0.5;
        std::vector<ColouredPoint> next;
        for (int j = 0; j < Lights.size(); j++) {
            for (size_t i = 0; i < Figures.size(); i++) {
                Point normal1 = Figures[i]->getNormal(source.point, Lights[j]);
                Point normal2 = Figures[i]->getNormal(source.point, A);
                if (areCoDirected(normal1,normal2)) {
                    next = Figures[i]->getPointOfIntersecting(ans, Lights[j]);
                    if (next.size() == 0) {
                        lightness += 2 / ((Lights[j] - source.point).len2());
                    }
                }
            }
        }
        source.colour = source.colour * lightness;
        return source;
    }

    ColouredPoint getMirrorColour(ColouredPoint source, Point A, size_t number) {
        Point ray = mirrorRay(source.point - A, Figures[number]->getNormal(source.point, A));
        Point s = source.point + ray/(ray.len()) * eps;

        std::cout << "source " << s.x << " "<< s.y << " "<< s.z << std::endl;
        std::vector<ColouredPoint> next;
        std::vector<ColouredPoint> answer;
        lld k = Figures[number]->m();
        //std::cout << k <<std::endl;
        lld cur_len = 10e7;
        Colour c = background_colour;

        for (size_t j = 0; j < Figures.size(); j++) {
            next = Figures[j]->getPointOfIntersecting(s, s + ray);
            answer.insert(answer.end(), next.begin(), next.end());
            for (size_t i = 0; i < next.size(); i++) {
                if ((next[i].point - s).len() < cur_len) {
                    cur_len = (next[i].point - s).len();
                  //  c = (getFoton(next[i], source.point)).colour;
                    c = next[i].colour;
                    std::cout << c.r << " "<< c.g << " "<< c.b << std::endl;
                }
            }
        }
        source.colour = source.colour*(1-k) + c*k ;
        c = source.colour;
        std::cout << "c " << c.r << " "<< c.g << " "<< c.b << std::endl;
        return ColouredPoint(source.point, c);
    }


 public:
    Colour background_colour;

    Scene(Colour bc, const std::vector<std::shared_ptr<Object>> &Fig = {}, const std::vector<Point> &L = {}) :
        Figures(Fig), Lights(L), background_colour(bc) {}

    void addObject(const std::shared_ptr<Object> &obj) {
        Figures.push_back(obj);
    }

    void addLights(const Point &light) {
        Lights.push_back(light);
    }

    std::vector<ColouredPoint> getPointsOfIntersecting(Point A, Point B) {
        std::vector<ColouredPoint> answer;
        std::vector<size_t> numbers;
        for (size_t i = 0; i < Figures.size(); i++) {
            std::vector<ColouredPoint> next = Figures[i]->getPointOfIntersecting(A, B);
            for (int j = 0; j < next.size(); j++) {
                next[j] = getMirrorColour(next[j],A,i);
               // next[j] = getFoton(next[j], A);
                numbers.push_back(i);
            }
            answer.insert(answer.end(), next.begin(), next.end());
        }
        return answer;
    }
};

struct Camera {
    Point Focus;
    Point A;
    Point B;
    Point C;
    Point D;
    //lld alpha, beta, gamma;
    //Camera(Point l, Point d, lld a, lld b, lld y): location(l), direction(d), alpha(a), beta(b), gamma(y){}

    Camera(Point f, Point a, Point b, Point c, Point d) : Focus(f), A(a), B(b), C(c), D(d) {}
};
