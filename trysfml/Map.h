#pragma once
#include <SFML/Graphics.hpp>

class Map
{
public:

	float squareSize;
	int sx, sy, mapW, mapH;

	Map(int sx, int sy, int mapW, int mapH, int *mapList);
	bool isWall(int x, int y);
	int wallType(int x, int y);
	void Draw(sf::RenderWindow& window, float mapScale);


private:
	int *mapList;
	int mapSize;
};

