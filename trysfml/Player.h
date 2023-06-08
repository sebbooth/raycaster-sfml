#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"


class Player
{
public:
	Player(sf::Texture* texture, sf::Vector2f pos, float angle);

	void Update(float deltaTime, Map& map);
	void Draw(sf::RenderWindow& window, float mapScale);
	void RayCast(sf::RenderWindow& window, Map& map, float mapScale);
	void CheckWallCollisions(Map& map);
	
	float FOV = 80.0f;
	float angle;
	sf::Vector2f pos;



private:
	void addColumn(sf::VertexArray& walls, int rayNum, int sx, int sy, float pixelX, float rayDist, float textCoord, sf::Texture& wallTexture, sf::Color wallColor);

	sf::RectangleShape body;
	float PLAYER_ROTATION_SPEED = 200.0f;
	float PLAYER_MOVEMENT_SPEED = 200.0f;
	float PLAYER_SIZE = 30.0f;
};

