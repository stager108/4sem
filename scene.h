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
    std::vector<LightPoint> Lights;
    Colour getFoton(ColouredPoint source, Point A) {
        Point ans = source.point + (A - source.point)/((A - source.point).len()) *eps;
        lld lightness = 0.5;
        std::vector<ColouredPoint> next;
        for (int j = 0; j < Lights.size(); j++) {
            for (size_t i = 0; i < Figures.size(); i++) {
                Point normal1 = Figures[i]->getNormal(source.point, Lights[j].location);
                Point normal2 = Figures[i]->getNormal(source.point, A);
                if (areCoDirected(normal1,normal2)) {
                    next = Figures[i]->getPointOfIntersecting(ans, Lights[j].location);
                    if (next.size() == 0) {
                        lightness += Lights[j].k/ ((Lights[j].location - source.point).len2()) *\
                         fabsl(normal1 * (source.point - Lights[j].location) / (source.point- Lights[j].location).len() / normal1.len());
                    }
                }
            }
        }
        source.colour = source.colour * lightness;
        return source.colour;
    }

    Colour getAnotherColour(ColouredPoint source, Point A, size_t number, char e, size_t count) {
        if(count > 4)
            return source.colour;

        Point ray;
        switch(e){
            case 'm': ray = mirrorRay(source.point - A, Figures[number]->getNormal(source.point, A)); break;
            case 'b': ray = brokenRay(source.point - A, Figures[number]->getNormal(source.point, A), 1, Figures[number]->k()); break;
        }

        Point s = source.point + ray/(ray.len()) * eps;
        std::vector<ColouredPoint> next;
        lld k = Figures[number]->m();
        lld cur_len = 100000000;
        size_t  n;
        Colour c = background_colour;

        for (size_t j = 0; j < Figures.size(); j++) {
            next = Figures[j]->getPointOfIntersecting(s, s + ray);
            for (size_t i = 0; i < next.size(); i++) {
                if ((next[i].point - source.point).len() < cur_len) {
                    cur_len = (next[i].point - s).len();
                    n = j;
                    Colour c1 = getAnotherColour(next[i], s, j ,'m', count+1);
                    Colour c2 = getAnotherColour(next[i], s, j ,'b', count+1);

                    c = (next[i].colour) * (1.0 - Figures[number]->m() - Figures[number]->c()) + c1 * (Figures[number]->m()) + c2*(Figures[number]->c());
                }
            }
        }
        return c;
    }

 public:
    Colour background_colour;

    Scene(Colour bc, const std::vector<std::shared_ptr<Object>> &Fig = {}, const std::vector<LightPoint> &L = {}) :
        Figures(Fig), Lights(L), background_colour(bc) {}

    void addObject(const std::shared_ptr<Object> &obj) {
        Figures.push_back(obj);
    }

    void addLights(const LightPoint &light) {
        Lights.push_back(light);
    }

    std::vector<ColouredPoint> getPointsOfIntersecting(Point A, Point B) {
        std::vector<ColouredPoint> answer;
        lld cur_len = 100000;
        ColouredPoint cur_point;
        size_t number = 0;
        int flag = 0;
        for (size_t i = 0; i < Figures.size(); i++) {
            std::vector<ColouredPoint>  next = Figures[i]->getPointOfIntersecting(A, B);
            for (int j = 0; j < next.size(); j++) {
                flag = 1;
                if ((next[j].point - A).len() < cur_len) {
                    cur_len = (next[j].point - A).len();
                    cur_point = next[j];
                    number = i;
                   // std::cout << " rt " <<  cur_point.colour.r << " "<<  cur_point.colour.g << " "<<  cur_point.colour.b << std::endl;
                }
            }
        }

        if (flag == 1){
            Colour c1 = getAnotherColour(cur_point, A, number, 'm', 1);
            Colour c2 = getAnotherColour(cur_point, A, number, 'b', 1);
            lld g = (1.0 - Figures[number]->m() - Figures[number]->c());
            cur_point.colour = (cur_point.colour) * g + c1 * (Figures[number]->m()) + c2*(Figures[number]->c());
            cur_point.colour = getFoton(cur_point, A);
            answer.push_back(cur_point);
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
