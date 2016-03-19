
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


const std::string cornerIntersectionFile = "corner.png";
const std::string edgeIntersectionFile = "edge.png";
const std::string intersectionFile = "intersection.png";
const std::string stoneFile = "stone.png";

class Tile
{
public:
    sf::Sprite board;
    int stone;  // 0 = board; 1 = black; 2 = white
    static sf::Sprite blackStone;
    static sf::Sprite whiteStone;

    static sf::Texture cornerImg;
    static sf::Texture edgeImg;
    static sf::Texture intersectionImg;
    static sf::Texture stoneImg;

    Tile();
    void setTexture(sf::Texture *tex);
    void setPosition(int x, int y);
    void setRotation(float angle);
    void placeStone(int player);
    void drawTile(sf::RenderWindow *window);
};

// define static Tile members
sf::Texture Tile::cornerImg;
sf::Texture Tile::edgeImg;
sf::Texture Tile::intersectionImg;
sf::Texture Tile::stoneImg;

sf::Sprite Tile::blackStone;
sf::Sprite Tile::whiteStone;

// Tile member function declarations
Tile::Tile(void)
{
    this->stone = 0;
    this->board.setOrigin(TILE_SIZE / 2, TILE_SIZE / 2);

    // set textures for stones
    Tile::blackStone.setTexture(Tile::stoneImg);
    Tile::whiteStone.setTexture(Tile::stoneImg);

    // color black stone sprite
    Tile::blackStone.setColor(sf::Color::Black);

    // set origins for stone sprites
    Tile::blackStone.setOrigin(TILE_SIZE / 2, TILE_SIZE /2);
    Tile::whiteStone.setOrigin(TILE_SIZE / 2, TILE_SIZE /2);
}

void Tile::setTexture(sf::Texture *tex)
{
    this->board.setTexture(*tex);
}

void Tile::setPosition(int x, int y)
{
    this->board.setPosition(x, y);
}

void Tile::setRotation(float angle)
{
    this->board.setRotation(angle);
}

void Tile::placeStone(int player)
{
    this->stone = player;
    if(player == 1)
    {
        Tile::blackStone.setPosition(this->board.getPosition());
    }
    else
    {
        Tile::whiteStone.setPosition(this->board.getPosition());
    }
}

void Tile::drawTile(sf::RenderWindow *window)
{

    window->draw(this->board);
    if(this->stone != 0)
    {
        this->placeStone(this->stone);
        if(this->stone == 1)
        {
            window->draw(Tile::blackStone);
        }
        else // this->stone == 2
        {
            //std::cout << "draw white stone\n";
            window->draw(Tile::whiteStone);
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(GAME_WIDTH * TILE_SIZE, GAME_HEIGHT * TILE_SIZE), "Go");

    sf::Time dt;
    sf::Time t;
    sf::Clock clock;

    int turn = 1;

    if(!Tile::cornerImg.loadFromFile(cornerIntersectionFile))
    {
        std::cout << "Failed to load corner image file\n";
        return 1;
    }
    if(!Tile::edgeImg.loadFromFile(edgeIntersectionFile))
    {
        std::cout << "Failed to load edge image file\n";
        return 1;
    }
    if(!Tile::intersectionImg.loadFromFile(intersectionFile))
    {
        std::cout << "Failed to load intersection image file\n";
        return 1;
    }
    if(!Tile::stoneImg.loadFromFile(stoneFile))
    {
        std::cout << "Failed to load stone image file\n";
        return 1;
    }

    Tile game[GAME_WIDTH][GAME_HEIGHT];

    window.clear();

    //initialize textures and draw the board.
    for(int i = 0; i < GAME_WIDTH; i++)
    {
        for(int j = 0; j < GAME_WIDTH; j++)
        {
            if(i == 0 && j == 0)
            {
                game[i][j].setTexture(&Tile::cornerImg);
            }
            else if(i == 0 && j == GAME_HEIGHT - 1)
            {
                game[i][j].setTexture(&Tile::cornerImg);
                game[i][j].setRotation(270);
            }
            else if(i == GAME_WIDTH - 1 && j == 0)
            {
                game[i][j].setTexture(&Tile::cornerImg);
                game[i][j].setRotation(90);
            }
            else if(i == GAME_WIDTH - 1 && j == GAME_HEIGHT - 1)
            {
                game[i][j].setTexture(&Tile::cornerImg);
                game[i][j].setRotation(180);
            }
            else if (i == 0)
            {
                game[i][j].setTexture(&Tile::edgeImg);
                game[i][j].setRotation(270);
            }
            else if (i == GAME_WIDTH - 1)
            {
                game[i][j].setTexture(&Tile::edgeImg);
                game[i][j].setRotation(90);
            }
            else if (j == 0)
            {
                game[i][j].setTexture(&Tile::edgeImg);
            }
            else if (j == GAME_HEIGHT - 1)
            {
                game[i][j].setTexture(&Tile::edgeImg);
                game[i][j].setRotation(180);
            }
            else
            {
                game[i][j].setTexture(&Tile::intersectionImg);
            }

            game[i][j].setPosition(i * TILE_SIZE + (TILE_SIZE / 2), j * TILE_SIZE + (TILE_SIZE / 2));
            game[i][j].drawTile(&window);
        }
    }

    window.draw(Tile::blackStone);

    window.display();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed)
            {
                int x = event.mouseButton.x / TILE_SIZE;
                int y = event.mouseButton.y / TILE_SIZE;

                //std::cout << x << ", " << y << ": " << (turn-1) % 2 + 1 << std::endl;

                game[x][y].placeStone((turn-1) % 2 + 1);
                turn++;
            }
        }

        // movement
        dt = clock.restart();
        t += dt;
        if(t.asMilliseconds() >= 100){

            t = sf::Time::Zero;

            // draw
            window.clear();
            for(int i = 0; i < GAME_WIDTH; i++)
            {
                for(int j = 0; j < GAME_WIDTH; j++)
                {
                    game[i][j].drawTile(&window);
                }
            }
            window.display();
        }

    }

    return 0;
}
