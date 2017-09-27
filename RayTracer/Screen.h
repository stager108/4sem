#pragma once

#include <SFML/Graphics.hpp>

struct Vector2d {
    int x, y;
};

class Screen {
public:

    Screen(int width, int height){
        img.create(width, height);
    }

    void putPixel(const Vector2d position, const Colour color) {
        img.setPixel(position.x, position.y, sf::Color(color.r, color.g, color.b));
    }

    const sf::Image getImage() { return img; }

    unsigned int getHeight() { return img.getSize().y; }
    unsigned int getWidth() { return img.getSize().x; }

private:
    sf::Image img;

};
