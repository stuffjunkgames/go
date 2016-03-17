
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <iostream>

#define GAME_TOP 0
#define GAME_LEFT 0
#define GAME_WIDTH 19
#define GAME_HEIGHT 19
const int TILE_SIZE = 50;
#define GAME_RIGHT GAME_LEFT+GAME_WIDTH
#define GAME_BOTTOM GAME_TOP+GAME_HEIGHT


static const std::string cornerIntersectionFile = "corner.tif";
static const std::string edgeIntersectionFile = "edge.tif";
static const std::string intersectionFile = "intersection.tif";
static const std::string stoneFile = "stone.tif";


int main()
{
    sf::RenderWindow window(sf::VideoMode(GAME_WIDTH * TILE_SIZE, GAME_HEIGHT * TILE_SIZE), "Go");

    sf::Time dt;
    sf::Time t;
    sf::Clock clock;

    sf::Image cornerImg;
    sf::Image edgeImg;
    sf::Image intersectionImg;
    sf::Image stoneImg;

    if(!cornerImg.loadFromFile(cornerIntersectionFile))
    {
        std::cout << "Failed to load corner image file\n";
        return 1;
    }

    if(!edgeImg.loadFromFile(edgeIntersectionFile))
    {
        std::cout << "Failed to load edge image file\n";
        return 1;
    }
    if(!intersectionImg.loadFromFile(intersectionFile))
    {
        std::cout << "Failed to load intersection image file\n";
        return 1;
    }
    if(!stoneImg.loadFromFile(stoneFile))
    {
        std::cout << "Failed to load stone image file\n";
        return 1;
    }

    sf::Sprite background[GAME_WIDTH][GAME_HEIGHT];

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
