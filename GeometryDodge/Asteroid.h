#pragma once
#include <SFML/Graphics.hpp>

class Asteroid
{
public:
	Asteroid(sf::RenderWindow* hwnd);
	~Asteroid();

	void update(float dt);
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

	float randSpeed;		// Add this later if the MVP works
	int randXPos;
	int randYPos;
};