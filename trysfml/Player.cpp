#include "Player.h"
#include <math.h>      
#include <iostream>
#include <algorithm>
#define PI 3.14159265f



Player::Player(sf::Texture* texture, sf::Vector2f pos, float angle)
{
	this->angle = angle;
	this->pos = pos;

	body.setTexture(texture);
	body.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(pos.x, pos.y);
	body.setTexture(texture);
	body.setRotation(angle);
}

void Player::Update(float deltaTime, Map& map)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		angle -= PLAYER_ROTATION_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		angle += PLAYER_ROTATION_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		pos.x += sin(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime;
		pos.y -= cos(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		pos.x -= sin(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime;
		pos.y += cos(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		pos.y += sin(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime; 
		pos.x += cos(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime;

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		pos.y -= sin(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime;
		pos.x -= cos(angle * PI / 180) * PLAYER_MOVEMENT_SPEED * deltaTime;

	}
	this->CheckWallCollisions(map);
	this->CheckWallCollisions(map);

}
	
void Player::Draw(sf::RenderWindow& window, float mapScale)
{
	body.setPosition(pos.x, pos.y);
	body.setRotation(angle);
	sf::Transform T;
	T.scale(mapScale, mapScale);
	window.draw(body, T);
}




void Player::RayCast(sf::RenderWindow& window, Map& map, float mapScale)	//TODO: shrink this function down
{
	float squareSize = map.squareSize;
	float offset = 0.0001f;						//to ensure checking collision with right square
	int posGridX = (int) (pos.x / squareSize);
	int posGridY = (int) (pos.y / squareSize);
	int mapWidth = map.mapW;
	int mapHeight = map.mapH;
	int sx = map.sx;
	int sy = map.sy;
	int rayNum, squareX, squareY, wallType, xWallType = 1, yWallType = 1;
	float pixelX = window.getSize().x / sx;		//TODO: fix issue with resizing
	bool xHit = false, yHit = false;

	float xTextCoord = 0.0f;
	float yTextCoord = 0.0f;
	float textCoord = 0.0f;
	
	sf::VertexArray ray(sf::TriangleFan, sx+1);
	sf::VertexArray walls(sf::Quads, sx * 4);
	sf::VertexArray walls1(sf::Quads, sx * 4);

	ray[0].position = pos;
	ray[0].color = sf::Color::White;

	sf::Texture wallTexture;
	sf::Texture wallTexture1;

	wallTexture.loadFromFile("images/wall2.png");
	wallTexture1.loadFromFile("images/wall3.png");

	for (rayNum = 0; rayNum < sx; rayNum++)
	{
		float rayAngle = angle - FOV / 2.0f + (FOV / sx) * rayNum;
		sf::Vector2f rayDir = sf::Vector2f(sin(rayAngle * PI / 180), -cos(rayAngle * PI / 180));

		float rayDistX = INFINITY, rayDistY = INFINITY, rayDist;


		if (rayDir.x >= 0) { rayDistX = (posGridX + 1) * squareSize - pos.x; }
		else { rayDistX = pos.x - (posGridX)*squareSize; }
		rayDistX /= abs(rayDir.x);
		sf::Vector2f checkPoint = pos + rayDir * rayDistX;
		squareX = (int)(checkPoint.x / squareSize);
		squareY = (int)(checkPoint.y / squareSize);
		if (rayDir.x < 0) { squareX--; }
		int checkNum = 1;
		while (squareX < mapWidth && squareX >= 0 && squareY < mapHeight && squareY >= 0)
		{
			xWallType = map.wallType(squareX, squareY);
			if (map.isWall(squareX, squareY))
			{
				xHit = true;
				break;
			}
			else
			{
				if (rayDir.x >= 0)
				{
					rayDistX = (posGridX + 1 + checkNum) * squareSize - pos.x + offset;

				}
				else
				{
					rayDistX = pos.x - (posGridX - checkNum) * squareSize - offset;

				}
				rayDistX /= abs(rayDir.x);
				checkPoint = pos + rayDir * rayDistX;

				squareX = (int)(checkPoint.x / squareSize);
				squareY = (int)(checkPoint.y / squareSize);
				if (rayDir.x < 0) { squareX--; }
			}
			checkNum++;
		}
		

		if (rayDir.y >= 0) { rayDistY = (posGridY + 1) * squareSize - pos.y; }
		else { rayDistY = pos.y - (posGridY)*squareSize; }
		rayDistY /= abs(rayDir.y);
		checkPoint = pos + rayDir * rayDistY;
		squareX = (int)(checkPoint.x / squareSize);
		squareY = (int)(checkPoint.y / squareSize);
		if (rayDir.y < 0) { squareY--; };
		checkNum = 1;
		while (squareX < mapWidth && squareX >= 0 && squareY < mapHeight && squareY >= 0)
		{
			yWallType = map.wallType(squareX, squareY);

			if (map.isWall(squareX, squareY)) 
			{ 
				yHit = true;
				break; 
			}
			else
			{
				if (rayDir.y >= 0)
				{
					rayDistY = (posGridY + 1 + checkNum) * squareSize - pos.y + offset;
				}
				else
				{
					rayDistY = pos.y - (posGridY - checkNum) * squareSize - offset;

				}
				rayDistY /= abs(rayDir.y);
				checkPoint = pos + rayDir * rayDistY;
				squareX = (int)(checkPoint.x / squareSize);
				squareY = (int)(checkPoint.y / squareSize);

				if (rayDir.y < 0) { squareY--; }
			}
			checkNum++;
		}



		rayDist = std::min(rayDistX, rayDistY);
		
		int maxView = 600;  //how far can see?
		int rayCol = (int)(1 - 255 * std::min(1.0f, (rayDist / maxView)));
		checkPoint = pos + rayDir * rayDist;

		if (rayDist == rayDistY)
		{
			rayCol = (int)(rayCol * 0.6);
			textCoord = (int)(checkPoint).x % (int) squareSize;
			wallType = yWallType;
		}
		else {
			textCoord = (int)(checkPoint).y % (int)squareSize;
			wallType = xWallType;
		}
		textCoord /= squareSize;
		squareX = (int)(checkPoint.x / squareSize);
		squareY = (int)(checkPoint.y / squareSize);
		sf::Color wallColor(rayCol, rayCol, rayCol);

		if (xHit || yHit) 
		{
			if (wallType == 1)
			{
				addColumn(walls, rayNum, sx, sy, pixelX, rayDist, textCoord, wallTexture, wallColor);
			}
			else if (wallType == 2)
			{
				addColumn(walls1, rayNum, sx, sy, pixelX, rayDist, textCoord, wallTexture1, wallColor);
			}
		}
		
	
		float rayColor = 200 - 200 * rayDist / sx;
		ray[rayNum + 1].position = pos + rayDir * rayDist;
		ray[rayNum + 1].color = sf::Color(rayColor, rayColor, rayColor);
	}




	sf::Transform T;
	T.scale(mapScale, mapScale);
	window.draw(walls, &wallTexture);
	window.draw(walls1, &wallTexture1);


	map.Draw(window, mapScale);
	this->Draw(window, mapScale);
	window.draw(ray, T);

}


void Player::addColumn(sf::VertexArray& walls, int rayNum, int sx, int sy, float pixelX, float rayDist, float textCoord, sf::Texture& wallTexture, sf::Color wallColor)
{
	walls[rayNum * 4].position = sf::Vector2f((float)rayNum, sy / 2 + sy * 22 / rayDist);
	walls[rayNum * 4 + 1].position = sf::Vector2f((float)rayNum + pixelX, sy / 2 + sy * 22 / rayDist);
	walls[rayNum * 4 + 2].position = sf::Vector2f((float)rayNum + pixelX, sy / 2 - sy * 22 / rayDist);
	walls[rayNum * 4 + 3].position = sf::Vector2f((float)rayNum, sy / 2 - sy * 22 / rayDist);

	walls[rayNum * 4].texCoords = sf::Vector2f(textCoord * wallTexture.getSize().x, 0.0f);
	walls[rayNum * 4 + 1].texCoords = sf::Vector2f(1 + textCoord * wallTexture.getSize().x, 0.0f);
	walls[rayNum * 4 + 2].texCoords = sf::Vector2f(1 + textCoord * wallTexture.getSize().x, wallTexture.getSize().y);
	walls[rayNum * 4 + 3].texCoords = sf::Vector2f(textCoord * wallTexture.getSize().x, wallTexture.getSize().y);

	walls[rayNum * 4].color = wallColor;
	walls[rayNum * 4 + 1].color = wallColor;
	walls[rayNum * 4 + 2].color = wallColor;
	walls[rayNum * 4 + 3].color = wallColor;
}

void Player::CheckWallCollisions(Map& map)
{
	int squareX, squareY;

	sf::Vector2f topL = sf::Vector2f(pos.x - PLAYER_SIZE / 2, pos.y - PLAYER_SIZE / 2);
	sf::Vector2f topR = sf::Vector2f(pos.x + PLAYER_SIZE / 2, pos.y - PLAYER_SIZE / 2);
	sf::Vector2f botL = sf::Vector2f(pos.x - PLAYER_SIZE / 2, pos.y + PLAYER_SIZE / 2);
	sf::Vector2f botR = sf::Vector2f(pos.x + PLAYER_SIZE / 2, pos.y + PLAYER_SIZE / 2);

	//TODO: can I put this into a loop/sub-function?
	squareX = (int)(topL.x / map.squareSize);
	squareY = (int)(topL.y / map.squareSize);
	if (map.isWall(squareX, squareY))
	{
		//std::cout << map.wallType(squareX, squareY) << " "<<squareX << " " << squareY<< std::endl;

		float diffX = map.squareSize * (squareX + 1) - topL.x;
		float diffY = map.squareSize * (squareY + 1) - topL.y;

		if (abs(diffX) < abs(diffY)) { pos.x += diffX; }
		else { pos.y += diffY; }
		return;
	}

	squareX = (int)(topR.x / map.squareSize);
	squareY = (int)(topR.y / map.squareSize);
	if (map.isWall(squareX, squareY))
	{
		//std::cout << map.wallType(squareX, squareY) << " "<<squareX << " " << squareY<< std::endl;

		float diffX = map.squareSize * (squareX) - topR.x;
		float diffY = map.squareSize * (squareY + 1) - topR.y;

		if (abs(diffX) < abs(diffY)) { pos.x += diffX; }
		else { pos.y += diffY; }
		return;

	}

	squareX = (int)(botL.x / map.squareSize);
	squareY = (int)(botL.y / map.squareSize);
	if (map.isWall(squareX, squareY))
	{
		//std::cout << map.wallType(squareX, squareY) << " "<<squareX << " " << squareY<< std::endl;

		float diffX = map.squareSize * (squareX+1)-botL.x;
		float diffY = map.squareSize * (squareY) - botL.y;

		if (abs(diffX) < abs(diffY)) { pos.x += diffX; }
		else { pos.y += diffY; }
		return;

	}

	squareX = (int)(botR.x / map.squareSize);
	squareY = (int)(botR.y / map.squareSize);
	if (map.isWall(squareX, squareY))
	{
		//std::cout << map.wallType(squareX, squareY) << " "<<squareX << " " << squareY<< std::endl;

		float diffX = map.squareSize * (squareX) - botR.x;
		float diffY = map.squareSize * (squareY) - botR.y;

		if (abs(diffX) < abs(diffY)) { pos.x += diffX; }
		else { pos.y += diffY; }
		return;

	}
}
