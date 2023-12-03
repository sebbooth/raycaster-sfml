#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include "Player.h"
#include "Map.h"
#include "DrawFunctions.h"

#define SX 1080
#define SY 720
#define MAPSCALE 0.3

void resizeView(const sf::RenderWindow& window, sf::View& view) 
{
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    if (aspectRatio > ((float)SX / (float)SY))
        view.setSize(SY * aspectRatio, SY);
    else
        view.setSize(SX, SX / aspectRatio);
}

int main()
{
    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(SX, SY), "SFML Raycasting");
    sf::View view(sf::Vector2f(SX / 2, SY / 2), sf::Vector2f(SX, SY));

    sf::Font font;
    font.loadFromFile("fonts/font.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setPosition(0, SY - 24);
    text.setFillColor(sf::Color::Black);

    sf::Texture texture;
    texture.loadFromFile("images/sprite.png");

    int mapWidth = 15;
    int mapHeight = 10;
    int squareWidth = SX / mapWidth;
    int squareHeight = SY / mapHeight;
    int mapList[] =
    {
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,2,0,0,0,2,0,0,0,0,1,
        1,0,2,0,0,0,0,2,0,0,2,0,2,0,1,
        1,0,2,0,0,2,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,2,0,0,0,0,2,0,2,0,1,
        1,0,0,0,0,0,0,0,2,2,2,0,0,0,1,
        1,0,1,0,0,0,0,0,0,0,2,0,0,0,1,
        1,0,1,0,0,0,0,0,0,0,0,0,2,0,1,
        1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
    };

    Player player(&texture, sf::Vector2f(SX/2, SY/2), 0.0f);
    Map map((int)SX, (int)SY, mapWidth, mapHeight, mapList);

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        int fps = 1 / deltaTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (event.type == sf::Event::Resized)
                resizeView(window, view);
        }
        window.setView(view);
        window.clear();

        drawBackGround(window, SX, SY);
        //drawFloor(window, player, map, SX, SY);   //Really not working for me
        player.Update(deltaTime, map);
        player.RayCast(window, map, MAPSCALE);

        std::stringstream ss1;   //find prettier way of converting int to string cause this is ugly af
        ss1 << " FPS:" << fps;
        std::string str1 = ss1.str();
        text.setString(str1);
        window.draw(text);

        window.display();    
    }
    return 0;
}