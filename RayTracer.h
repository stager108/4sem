#pragma once
#include "primitive.h"
#include "scene.h"
#include "Screen.h"

class RayTracer {

 public:
    RayTracer(Screen &screen, Scene &scene, Camera &camera) :
        screen(screen), scene(scene), camera(camera) {}

    std::vector<Colour> render() {
        std::vector<Colour> ans;
        //test 0
        //scene.addLights(Point(1,0.7,1));
        //scene.addObject(std::shared_ptr<Object>(new Triangle({2.5, 1, 0}, {2.5, 0.75, 1}, {2.0, 0, 0.75}, {17,0,120},{17,0,120}, {0, 50, 1},1)));

         //scene.addObject(std::shared_ptr<Object>(new Sphere(0.5, {4,0.5,0}, {0,0,55}, {0.5, 0, 0})));
        //scene.addObject(std::shared_ptr<Object>(new Triangle({2.5, 1.6, 1.6}, {2.2, 1.6, 0.9}, {2.2, 0.7, 1.6}, {120, 0, 0}, {0,120, 0}, {0, 50, 1.2},1)));
       // scene.addObject(std::shared_ptr<Object>(new Quadrilateral({3,0.4,0.4}, {3,0.4,5.9}, {3,1.9,5.9},{3,1.9, 0.4}, {0,0,255}, {0,0,255}, {70, 0, 0},1)));
        // scene.addObject(std::shared_ptr<Object>(new Sphere(0.5, {4,0.5,1}, {0,55,0}, {0,0, 0})));
        // scene.addLights(Point(1,0.7,5));
//
        //test 1
        //scene.addLights(Point(1,0,0));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.25, {4, 1,0.5}, {0,25,25}, {50, 0, 0})));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.5, {4,0.5,0}, {0,0,25}, {50, 0, 0})));
        //scene.addObject(std::shared_ptr<Object>(new Triangle({2.5, 0.6, 0.6}, {2.2, 0.6, -0.1}, {2.2, -0.3, 0.6}, {12, 12, 0},{12, 12, 0}, {0, 50, 1.2}, 1)));
        //scene.addObject(std::shared_ptr<Object>(new Quadrilateral({6,-3,-3}, {6,-3, 3}, {6, 3, 3},{6, 3, -3}, {45, 0, 45},  {45, 0, 45}, {0.3, 0, 0}, 1)));

        //test 2
        //scene.addLights(Point(1.5,0.75,0.75));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.25, {4, 1,0.5}, {0,25,25}, {50, 0, 0})));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.5, {4,0.5,0}, {0,0,25}, {50, 0, 0})));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.35, {4, 1.9,0.4}, {25,25,25}, {50, 0, 0})));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.75, {4,0.5,0.5}, {25,0,25}, {50, 40, 1})));
        //scene.addObject(std::shared_ptr<Object>(new Quadrilateral({6,-3,-3}, {6,-3, 3}, {6, 3, 3},{6, 3, -3}, {45, 0, 45},  {45, 0, 45}, {0.3, 0, 0}, 1)));


        // test 3
        //scene.addLights(Point(1.5,0.75,0.75));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.25, {4, 1,0.5}, {0,15,15}, {50, 0, 0})));
        //scene.addObject(std::shared_ptr<Object>(new Sphere(0.5, {4, 1, 0.5}, {25,0,0}, {50, 70, 1.2})));
        //scene.addObject(std::shared_ptr<Object>(new Quadrilateral({6,-3,-3}, {6,-3, 3}, {6, 3, 3},{6, 3, -3}, {45, 0, 45},  {45, 0, 45}, {0.3, 0, 0}, 1)));

        for (int i = 0; i < screen.getWidth(); i++) {
            for (int j = 0; j < screen.getHeight(); j++) {
                const Point ray = camera.A + i*dx + j*dy + dx/2 + dy/2 ;
                const Colour col = trace(camera.Focus, ray);
                screen.putPixel({i, j}, col);
            }
        }
        return ans;
    }

 public:
    Screen &screen;
    Scene &scene;
    Camera &camera;
    Point focus;
    lld screen_position;
    Point dx = (camera.C - camera.B)/(screen.getHeight());
    Point dy = (camera.C - camera.D)/(screen.getWidth());
    Colour trace(Point A, Point B) {
        Colour cur_colour = scene.background_colour;
        std::vector<ColouredPoint> points = scene.getPointsOfIntersecting(A, B);
        if (points.size() > 0) {
            lld cur_len = (points[0].point - camera.Focus).len();
            cur_colour = points[0].colour;
            /*for (int i = 1; i < points.size(); i++) {
                if ((points[i].point - camera.Focus).len() < cur_len) {
                    cur_len = (points[i].point - camera.Focus).len();
                    cur_colour = points[i].colour;
                }
            }*/
        }
        return cur_colour;
    }

};