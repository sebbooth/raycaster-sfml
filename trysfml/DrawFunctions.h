#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Map.h"

#define PI 3.14159265f

void drawFloor(sf::RenderWindow& window, Player player, Map map, int SX, int SY)
{
    int col;
    float halfSY = SY / 2;
    int pixelSize = 8;
    sf::VertexArray floor(sf::Quads, halfSY * SX);

    int pixel = 0;
    for (col = 0; col < SX; col += pixelSize)
    {
        float rayAngle = player.angle - player.FOV / 2.0f + (player.FOV / SX) * col;
        float sine = sin(rayAngle * PI / 180);
        float cosine = cos(rayAngle * PI / 180);

        int row;
        for (row = 0; row < halfSY; row += pixelSize)
        {
            float dist = halfSY / (row - halfSY);

            float x = (player.pos.x / map.squareSize) + sine * dist;
            float y = (player.pos.y / map.squareSize) + cosine * dist;

            if (int(x) % 2 == int(y) % 2)
            {
                floor[pixel].position = sf::Vector2f(col, SY - row);
                floor[pixel + 1].position = sf::Vector2f(col + pixelSize, SY - row);
                floor[pixel + 2].position = sf::Vector2f(col + pixelSize, SY - row + pixelSize);
                floor[pixel + 3].position = sf::Vector2f(col, SY - row + pixelSize);
                floor[pixel].color = sf::Color::White;
                floor[pixel + 1].color = sf::Color::White;
                floor[pixel + 2].color = sf::Color::White;
                floor[pixel + 3].color = sf::Color::White;

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
            pixel += 4;
        }
    }
    window.draw(floor);
}

void drawBackGround(sf::RenderWindow& window, int SX, int SY)
{
    sf::VertexArray ground(sf::Quads, 4);
    sf::VertexArray sky(sf::Quads, 4);

    sky[0].position = sf::Vector2f(0.0, 0.0);
    sky[1].position = sf::Vector2f(SX, 0.0);
    sky[2].position = sf::Vector2f(SX, SY / 2);
    sky[3].position = sf::Vector2f(0, SY / 2);

    sky[0].color = sf::Color(0, 0, 44);
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
