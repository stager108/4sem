#pragma once

#include <QtGui/QImage>
#include "colour.h"

struct Vector2d {
    int x, y;
};

class Screen {
    public:

    Screen(int width, int height) :img(width, height, QImage::Format_RGB32) {}

    void putPixel(const Vector2d position, const Colour color) {
        img.setPixelColor(position.x, position.y, QColor(color.r, color.g, color.b));
    }

    const QImage& getQImage() { return img; }

    QSize getSize() { return img.size(); }

    private:
    QImage img;
};
