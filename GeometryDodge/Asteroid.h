#pragma once
#include <SFML/Graphics.hpp>
#include "GameWorldData.h"

class Asteroid
{
public:
	Asteroid(sf::RenderWindow* hwnd);
	~Asteroid();

	void update(float dt, GameWorldData gwd);
	void render(sf::RenderWindow* window);
	sf::Sprite* getAsteroidSprite();
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height);

private:
	void initAsteroid();
	void loadTexture();
	void checkScreenBounds();
	void move();
	
	sf::RenderWindow* window;
	sf::Texture asteroidTexture;
	sf::Sprite asteroidSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2u size;
	sf::FloatRect collisionBox;

	float speed;
	int randXPos;
	int randYPos;
};