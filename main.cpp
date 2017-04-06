
#include <SFML/Graphics.hpp>
#include "RayTracer.h"
#include "Screen.h"

int main()
{
    unsigned int width = 800, height = 800;
    sf::RenderWindow window(sf::VideoMode(width, height), "RayTracer");
    Screen scr(width, height);
    Scene myscn(Colour(40, 40, 40));
    Camera camera(Point(0, 0.5, 0.5), Point(1, 0, 0), Point(1,0,1), Point(1,1,1), Point(1,1, 0));
    RayTracer RT(scr, myscn, camera);
    RT.render();

    window.clear();
    sf::Texture texture;
    texture.loadFromImage(scr.getImage());
    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    window.draw(sprite);
    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}
