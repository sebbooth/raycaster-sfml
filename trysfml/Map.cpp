#include "Map.h"
#include <iostream>

Map::Map(int sx, int sy, int mapW, int mapH, int *mapList)
{
    this->sx = sx;
    this->sy = sy;
    this->mapW = mapW;
    this->mapH = mapH;
    this->mapSize = mapW * mapH;
    this->squareSize = std::min(sx / mapW, sy / mapH);
    this->mapList = mapList;

}

bool Map::isWall(int x, int y)
{
    int index = x + y * mapW;
    if (index < 0 || index > mapSize)
    {
        return false;
    }
	return (mapList[index] != 0);
}

int Map::wallType(int x, int y)
{
    int index = x + y * mapW;
    if (index < 0 || index > mapSize)
    {
        return 0;
    }
    return (mapList[index]);
}



void Map::Draw(sf::RenderWindow& window, float mapScale)
{
    int i, j;
    sf::Transform T;
    T.scale(mapScale, mapScale);

    sf::VertexArray bg(sf::Quads, 4);
    bg[0].position = sf::Vector2f(0, 0);
    bg[1].position = sf::Vector2f(squareSize*mapW, 0);
    bg[2].position = sf::Vector2f(squareSize * mapW, squareSize * mapH);
    bg[3].position = sf::Vector2f(0, squareSize * mapH);
    bg[0].color = sf::Color::Black;
    bg[1].color = sf::Color::Black;
    bg[2].color = sf::Color::Black;
    bg[3].color = sf::Color::Black;
    window.draw(bg, T);

    for (i = 0; i < mapH; i++)
    {
        for (j = 0; j < mapW; j++)
        {
            sf::VertexArray square(sf::Quads, 4);
            sf::VertexArray outline(sf::LineStrip, 5);

            if (mapList[i * mapW + j] !=0)
            {
                square[0].position = sf::Vector2f(j * squareSize, i * squareSize);
                square[1].position = sf::Vector2f((j + 1) * squareSize, i * squareSize);
                square[2].position = sf::Vector2f((j + 1) * squareSize, (i + 1) * squareSize);
                square[3].position = sf::Vector2f(j * squareSize, (i + 1) * squareSize);
                outline[0].position = sf::Vector2f(j * squareSize, i * squareSize);
                outline[1].position = sf::Vector2f((j + 1) * squareSize, i * squareSize);
                outline[2].position = sf::Vector2f((j + 1) * squareSize, (i + 1) * squareSize);
                outline[3].position = sf::Vector2f(j * squareSize, (i + 1) * squareSize);
                outline[4].position = sf::Vector2f(j * squareSize, i * squareSize);
                outline[0].color = sf::Color::Black;
                outline[1].color = sf::Color::Black;
                outline[2].color = sf::Color::Black;
                outline[3].color = sf::Color::Black;
                outline[4].color = sf::Color::Black;

                window.draw(square, T);
                window.draw(outline, T);
            }
        }
    }
}
