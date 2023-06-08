#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Player.h"
#include "Map.h"
#include <algorithm>
#define PI 3.14159265f

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


void drawFloor(sf::RenderWindow& window, Player player, Map map)
{
    int col;
    float halfSY = SY / 2;
    int pixelSize = 8;
    sf::VertexArray floor(sf::Quads, halfSY*SX);

    int pixel = 0;
    for (col = 0; col < SX; col+=pixelSize)
    {
        float rayAngle = player.angle - player.FOV / 2.0f + (player.FOV / SX) * col;
        float sine = sin(rayAngle * PI / 180);
        float cosine = cos(rayAngle * PI / 180);

        int row;
        for (row = 0; row < halfSY; row+=pixelSize)
        {
            float dist = halfSY / (row - halfSY);

            float x = (player.pos.x/map.squareSize) + sine * dist;
            float y = (player.pos.y/map.squareSize) + cosine * dist;

            if (int(x) % 2 == int(y) % 2)
            {
                floor[pixel].position = sf::Vector2f(col, SY-row);
                floor[pixel+1].position = sf::Vector2f(col+pixelSize, SY-row);
                floor[pixel+2].position = sf::Vector2f(col+pixelSize, SY-row+pixelSize);
                floor[pixel+3].position = sf::Vector2f(col, SY-row+pixelSize);
                floor[pixel].color = sf::Color::White;
                floor[pixel+1].color = sf::Color::White;
                floor[pixel+2].color = sf::Color::White;
                floor[pixel+3].color = sf::Color::White;

            }
            else
            {
                floor[pixel].position = sf::Vector2f(col, SY - row);
                floor[pixel + 1].position = sf::Vector2f(col + pixelSize, SY - row);
                floor[pixel + 2].position = sf::Vector2f(col + pixelSize, SY - row + pixelSize);
                floor[pixel + 3].position = sf::Vector2f(col, SY - row + pixelSize);
                floor[pixel].color = sf::Color::Red;
                floor[pixel + 1].color = sf::Color::Red;
                floor[pixel + 2].color = sf::Color::Red;
                floor[pixel + 3].color = sf::Color::Red;
            }
            pixel+=4;
        }
    }
    window.draw(floor);
}

void drawBackGround(sf::RenderWindow& window)
{
    sf::VertexArray ground(sf::Quads, 4);
    sf::VertexArray sky(sf::Quads, 4);

    sky[0].position = sf::Vector2f(0.0, 0.0);
    sky[1].position = sf::Vector2f(SX, 0.0);
    sky[2].position = sf::Vector2f(SX, SY / 2);
    sky[3].position = sf::Vector2f(0, SY / 2);

    sky[0].color = sf::Color(0,0,44);
    sky[1].color = sf::Color(0, 0, 44);
    sky[2].color = sf::Color(0, 0, 44);
    sky[3].color = sf::Color(0, 0, 44);
    
    ground[0].position = sf::Vector2f(0.0, SY / 2);
    ground[1].position = sf::Vector2f(SX, SY / 2);
    ground[2].position = sf::Vector2f(SX, SY);
    ground[3].position = sf::Vector2f(0, SY);

    ground[0].color = sf::Color::Black;
    ground[1].color = sf::Color::Black;
    ground[2].color = sf::Color::Green;
    ground[3].color = sf::Color::Green;
    
    window.draw(sky);
    window.draw(ground);
}

int main()
{
    sf::Clock clock;
    
    
    sf::RenderWindow window(sf::VideoMode(SX, SY), "2D");
    sf::View view(sf::Vector2f(SX / 2, SY / 2), sf::Vector2f(SX, SY));

    //sf::RenderWindow window2(sf::VideoMode(SX, SY), "3D");
    //sf::View view2(sf::Vector2f(SX / 2, SY / 2), sf::Vector2f(SX, SY));

    

    sf::Font font;
    font.loadFromFile("fonts/font.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setPosition(0, SY - 24);
    text.setFillColor(sf::Color::Black);


    sf::Texture texture;
    texture.loadFromFile("images/sprite.png");

    


    //window.setVerticalSYncEnabled(true);


    int mapWidth = 15;
    int mapHeight = 10;
    int squareWidth = SX / mapWidth;
    int squareHeight = SY / mapHeight;

    int mapList[] =
    {
        1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
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
    std::cout << (sizeof(mapList) / sizeof(*mapList)) << "\n";
    int mapList2[] =
    { 
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    };

    Player player(&texture, sf::Vector2f(SX/2, SY/2), 0.0f);
    Map map((int)SX, (int)SY, mapWidth, mapHeight, mapList);
    Map map1((int)SX, (int)SY, mapWidth, mapHeight, mapList);
    Map map2((int)SX, (int)SY, mapWidth, mapHeight, mapList2);

    /*
    //Little idea for later
    if (player.angle > 360.0) {
        map = map2;
    }
    else {
        map = map1;
    }
    */

   

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        int fps = 1 / deltaTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                resizeView(window, view);
        }
        //window.setView(view);
        window.clear();


        drawBackGround(window);
        //drawFloor(window, player, map);
        player.Update(deltaTime, map);
        player.RayCast(window, map, MAPSCALE);


        //Show FPS
        //TODO: find prettier way of converting int to string cause this is ugly af
        std::stringstream ss1;
        ss1 << " FPS:" << fps;
        std::string str1 = ss1.str();
        text.setString(str1);
        window.draw(text);


        window.display();    
    }


    return 0;
}