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
	Colour background_colour;

public:
	Scene(Colour bc, const std::vector<std::shared_ptr<Object>> &Fig = {}, const std::vector<Point> &L = {}) :
			Figures(Fig),Lights(L), background_colour(bc) {}

	void addObject(const std::shared_ptr<Object>& obj) {
		Figures.push_back(obj);
	}

	void addLights(const Point &light) {
		Lights.push_back(light);
	}

	std::vector <ColouredPoint> getPointsOfIntersecting(Point A, Point B){
		int n = Figures.size();
		std::vector <ColouredPoint> answer;
		for(int i = 0; i < n ; i++){
			std::vector<ColouredPoint> next = Figures[i]->intersectedInPoint(A,B);
			answer.insert(answer.end(), next.begin(), next.end());
		}
		return answer;
	}
};

struct Camera{
	Point location;
	Point direction;
	//lld alpha, beta, gamma;
	//Camera(Point l, Point d, lld a, lld b, lld y): location(l), direction(d), alpha(a), beta(b), gamma(y){}

	Camera(Point l, Point d): location(l), direction(d){}
};