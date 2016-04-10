
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <cstdlib>
#include <iostream>

/* uncomment this to use server code instead of client code */
//#define SERVER
#define GAME_TOP 0
#define GAME_LEFT 0
#define GAME_WIDTH 19
#define GAME_HEIGHT 19
const int TILE_SIZE = 50;
#define GAME_RIGHT GAME_LEFT+GAME_WIDTH
#define GAME_BOTTOM GAME_TOP+GAME_HEIGHT

const int port = 12345;

#ifndef SERVER
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
    void removeStone();
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
//    if(player == 1)
//    {
//        Tile::blackStone.setPosition(this->board.getPosition());
//    }
//    else
//    {
//        Tile::whiteStone.setPosition(this->board.getPosition());
//    }
}

void Tile::removeStone()
{
    this->stone = 0;
}

void Tile::drawTile(sf::RenderWindow *window)
{

    window->draw(this->board);
    if(this->stone != 0)
    {
        this->placeStone(this->stone);
        if(this->stone == 1)
        {
            Tile::blackStone.setPosition(this->board.getPosition());
            window->draw(Tile::blackStone);
        }
        else // this->stone == 2
        {
            //std::cout << "draw white stone\n";
            Tile::whiteStone.setPosition(this->board.getPosition());
            window->draw(Tile::whiteStone);
        }
    }
}

bool makeMove(Tile game[GAME_WIDTH][GAME_HEIGHT], int x, int y, int player);
bool isCaptured(Tile game[GAME_WIDTH][GAME_HEIGHT], int x, int y, int captured[GAME_WIDTH][GAME_HEIGHT], int player);
bool isCapturedHelper(Tile game[GAME_WIDTH][GAME_HEIGHT], int x, int y, int captured[GAME_WIDTH][GAME_HEIGHT], int player);
void zeroArray(int a[GAME_WIDTH][GAME_HEIGHT]);
void captureStones(Tile game[GAME_WIDTH][GAME_HEIGHT], int captured[GAME_WIDTH][GAME_HEIGHT]);

#endif // SERVER

int main()
{
#ifdef SERVER
    // server code -- just pass packets through
    sf::TcpListener listener;
    sf::Packet packet;

    std::cout << "Binding listener to port " << port << std::endl;
    // bind listener to port
    if(listener.listen(port) != sf::Socket::Done)
    {
        std::cout << "Error listening to port!\n";
        return 1;
    }

    // accept a new connection
    sf::TcpSocket client1;
    sf::TcpSocket client2;
    std::cout << "Listening for client connections...\n";
    if(listener.accept(client1) != sf::Socket::Done || listener.accept(client2) != sf::Socket::Done)
    {
        std::cout << "Error accepting client!\n";
        return 1;
    }

    std::cout << "Both clients connected!\n";

    sf::Uint16 player = 1;
    sf::Uint16 x, y;
    packet << player;
    std::cout << "Sending to player 1\n";
    if(client1.send(packet) != sf::Socket::Done)
    {
        std::cout << "Error sending packet\n";
        return 1;
    }
    packet.clear();
    player = 2;
    packet << player;
    std::cout << "Sending to player 2\n";
    if(client2.send(packet) != sf::Socket::Done)
    {
        std::cout << "Error sending packet\n";
        return 1;
    }
    packet.clear();

    while(1)
    {
        if(client1.receive(packet) != sf::Socket::Done)
        {
            std::cout << "Error receiving packet\n";
            return 1;
        }
        if(client2.send(packet) != sf::Socket::Done)
        {
            std::cout << "Error sending packet\n";
            return 1;
        }
        packet >> x >> y >> player;
        std::cout << x << ", " << y << ", player " << player << std::endl;

        packet.clear();
        if(client2.receive(packet) != sf::Socket::Done)
        {
            std::cout << "Error receiving packet\n";
            return 1;
        }
        if(client1.send(packet) != sf::Socket::Done)
        {
            std::cout << "Error sending packet\n";
            return 1;
        }
        packet >> x >> y >> player;
        std::cout << x << ", " << y << ", player " << player << std::endl;

        packet.clear();
    }
#endif // SERVER


#ifndef SERVER
    std::cout << "Attempting to connect to server...\n";
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect("msquared169.ddns.net", port);
    sf::Packet packet;
    std::cout << "Connected socket\n";
    if(status != sf::Socket::Done)
    {
        std::cout << "Error connecting socket!\n";
        return 1;
    }

    // wait to see if first or second
    if(socket.receive(packet) != sf::Socket::Done)
    {
        std::cout << "Error receiving packet!\n";
        return 1;
    }
    sf::Uint16 player;
    packet >> player;
    if(player == 1)
    {
        std::cout << "You go first!\n";
    }
    else if(player == 2)
    {
        std::cout << "You go second!\n";
    }

    // start the game
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
        for(int j = 0; j < GAME_HEIGHT; j++)
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

    sf::Uint16 xMove, yMove, color;

    socket.setBlocking(false);
    std::cout << "Start!\n";
    while (window.isOpen())
    {
        sf::Event event;
        if(player == ((turn-1) % 2) + 1) // your turn
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if(event.type == sf::Event::MouseButtonPressed)
                {
                    xMove = event.mouseButton.x / TILE_SIZE;
                    yMove = event.mouseButton.y / TILE_SIZE;
                    color = (turn-1) % 2 + 1;

                    // need to check if it will capture something before determining if it is suicide
                    // put this inside isMoveLegal?
                    if(makeMove(game, xMove, yMove, color))
                    {
                        turn++;
                        packet.clear();
                        // send move to opponent
                        packet << xMove << yMove << color;
                        socket.send(packet);
                    }

                }
            }
        }
        else // other players turn
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if(event.type == sf::Event::MouseButtonPressed)
                {
                    // ignore mouse clicks while not your turn

                }
            }

            packet.clear();
            if(socket.receive(packet) == sf::Socket::Done)
            {
                packet >> xMove >> yMove >> color;

                makeMove(game, xMove, yMove, color);
                turn++;
            }

        }

        // timing
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
#endif // not SERVER
}

#ifndef SERVER // don't use any of this if server
// adds stone to board (if legal)
// removes any dead stones
// returns true if move at x,y is legal for player
bool makeMove(Tile game[GAME_WIDTH][GAME_HEIGHT], int x, int y, int player)
{
    if(game[x][y].stone != 0)
    {
        return 0;
    }

    game[x][y].placeStone(player);
    int captured[GAME_WIDTH][GAME_HEIGHT] = {0};

    bool legal = 0;

    if(isCaptured(game, x+1, y, captured, (player % 2) + 1))
    {
        legal = 1;
        captureStones(game, captured);
    }
    zeroArray(captured);
    if(isCaptured(game, x-1, y, captured, (player % 2) + 1))
    {
        legal = 1;
        captureStones(game, captured);
    }
    zeroArray(captured);
    if(isCaptured(game, x, y+1, captured, (player % 2) + 1))
    {
        legal = 1;
        captureStones(game, captured);
    }
    zeroArray(captured);
    if(isCaptured(game, x, y-1, captured, (player % 2) + 1))
    {
        legal = 1;
        captureStones(game, captured);
    }
    zeroArray(captured);

    if(legal)
    {
        return 1;
    }

    legal = !isCaptured(game, x, y, captured, player);
    if(!legal)
    {
        game[x][y].removeStone();
    }

    return legal;
}

// return 1 if group of stone at x, y is captured, 0 if not.
// also modifies array to indicate which stones are captured
// captured array must be initialized to 0
bool isCaptured(Tile game[GAME_WIDTH][GAME_HEIGHT], int x, int y, int captured[GAME_WIDTH][GAME_HEIGHT], int player)
{
    // return an error if x,y is outside the playing area or not occupied
    if(x < 0 || y < 0 || x >= GAME_WIDTH || y >= GAME_HEIGHT || game[x][y].stone != player)
    {
        return 0;
    }
//    else if(player != game[x][y].stone)
//    {
//        return 0;
//    }

    captured[x][y] = 1;

    return (isCapturedHelper(game, x-1, y, captured, player) && isCapturedHelper(game, x+1, y, captured, player) &&
            isCapturedHelper(game, x, y-1, captured, player) && isCapturedHelper(game, x, y+1, captured, player));
}

// recursive helper for isCaptured
bool isCapturedHelper(Tile game[GAME_WIDTH][GAME_HEIGHT], int x, int y, int captured[GAME_WIDTH][GAME_HEIGHT], int player)
{
    if(captured[x][y] == 1)
    {
        return 1;
    }

    if(x < 0 || y < 0 || x >= GAME_WIDTH || y >= GAME_HEIGHT)
    {
        return 1;
    }

    if(game[x][y].stone == (player % 2) + 1)
    {
        return 1;
    }

    if(game[x][y].stone == 0)
    {
        return 0;
    }

    captured[x][y] = 1;

    return (isCapturedHelper(game, x-1, y, captured, player) && isCapturedHelper(game, x+1, y, captured, player) &&
            isCapturedHelper(game, x, y-1, captured, player) && isCapturedHelper(game, x, y+1, captured, player));
}

// removes stones in captured array
void captureStones(Tile game[GAME_WIDTH][GAME_HEIGHT], int captured[GAME_WIDTH][GAME_HEIGHT])
{
    for(int i = 0; i < GAME_WIDTH; i++)
    {
        for(int j = 0; j < GAME_HEIGHT; j++)
        {
            if(captured[i][j] == 1)
            {
                game[i][j].removeStone();
            }
        }
    }
}

// zeroes all elements in the array a
void zeroArray(int a[GAME_WIDTH][GAME_HEIGHT])
{
    for(int i = 0; i < GAME_WIDTH; i++)
    {
        for(int j = 0; j < GAME_HEIGHT; j++)
        {
            a[i][j] = 0;
        }
    }
}

#endif // SERVER
