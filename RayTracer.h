#pragma once
#include "primitive.h"
#include "scene.h"
#include "Screen.h"
class RayTracer {

public:
	RayTracer(Screen& screen, Scene& scene, Camera& camera, Point focus, lld screen_position):
			screen(screen), scene(scene), camera(camera), focus(focus), screen_position(screen_position){}

	std::vector <Colour> render(int weight, int height) {
		std::vector <Colour> ans;
		for(int i = 0; i < weight; i++) {
			for(int j = 0; j < height; j++) {
				const lld x = i - dx;
				const lld y = j - dy;
				const Point ray = Point(x, y, screen_position);
				const Colour col = trace(focus, ray );
				screen.putPixel({i, j}, col);
				//ans[i*weight + height] = col;
			}
		}
		return ans;
	}

private:
	Screen& screen;
	Scene& scene;
	Camera& camera;
	Point focus;
	lld screen_position;
	lld dx;
	lld dy;
	Colour trace(Point A, Point B){
		std::vector<ColouredPoint> points = scene.getPointsOfIntersecting(A,B);
		lld cur_len = (points[0].point - camera.location).len();
		Colour cur_colour = points[0].colour;
		for (int i = 1; i < points.size(); i++){
				if((points[i].point - camera.location).len() < cur_len){
					cur_len = (points[i].point - camera.location).len();
					cur_colour = points[i].colour;
				}
		}
		return cur_colour;
	}


};