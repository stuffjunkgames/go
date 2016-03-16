
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <iostream>

#define GAME_TOP 0
#define GAME_LEFT 0
#define GAME_WIDTH 1024
#define GAME_HEIGHT 768
#define GAME_RIGHT GAME_LEFT+GAME_WIDTH
#define GAME_BOTTOM GAME_TOP+GAME_HEIGHT

public class BoardSection
{
public:
    static const std::string cornerIntersectionFile = "corner.tif";
    static const std::string edgeIntersectionFile = "edge.tif";
    static const std::string intersectionFile = "intersection.tif";
    static const std::string stoneFile = "stone.tif";
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Go");

    sf::Time dt;
    sf::Time t;
    sf::Clock clock;


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();


        }

        // movement
        dt = clock.restart();
        t += dt;
        if(t.asMilliseconds() >= 100){

            t = sf::Time::Zero;

            // draw
            window.clear();

            window.display();
        }

    }

    return 0;
}
