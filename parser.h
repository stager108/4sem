#pragma once
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "colour.h"
#include "geom_utils.h"
#include "structures.h"

using namespace std;

class Parser
{
public:
    Parser(string file):input(file.c_str()){}
    void read(Scene scene)
    {
        int N, M;
        input >> N >> M;
        for(int i = 0; i < N; ++i){
            int type;
            input >> type;
            int mirror;
            int clearance;
            double k;
            input >> mirror >> clearance >> k;
            int r, g, b;
            input >> r >> g >> b;
            Colour colour = Color(r, g, b);
            if (type == 3 || type == 4)
            {
                input >> r >> g >> b;
                Colour backcolour = Color(r, g, b);
                std::vector<Point> figure(3);
                for (int j = 0; j < type; ++j){
                    figure[j] = readPoint();
                }
                int normal;
                input >> normal;
                if (type == 3){
                    scene.addObjects(std::shared_ptr<Object>(new Triangle(figure[0], figure[1], figure[2],  \
                    colour, backcolour, {mirror, clearance, k}, normal)));
                }
                else{
                    scene.addObjects(std::shared_ptr<Object>(new Quadrilateral(figure[0], figure[1], figure[2], \
                    figure[3], colour, backcolour, {mirror, clearance, k}, normal)));
                }
            }
            else {
                Point center;
                double r;
                center = readPoint();
                input >> r;
                scene.addObjects(std::shared_ptr<Object>(new Sphere(r, center,color, {mirror, clearance, k})));
            }
        }
        for(int i = 0; i < M; ++i){
            Point place = readPoint();
            scene.addLights(new Point(place));
        }
    }
private:
    Point readPoint(){
        lld x, y, z;
        input >> x >> y >> z;
        return Point(x, y, z);
    }
    ifstream input;
};
