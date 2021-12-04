#pragma once
#include "GameState.h"
#include "Screen.h"
#include "Player.h"
#include "UI.h"
#include "Asteroid.h"

class Level : public Screen
{
public:
	Level() {}
	Level(sf::RenderWindow* hwnd, Input* controller_0, GameState* gs);
	~Level();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;

private:
	void initDebugMode();
	void initBackground();
	void initUI();
	void initLevel();
	void initPlayer();
	void initAsteroids();
	void spawnNewAsteroid();
	void checkCurrentWave();
	void checkCollisions();
	void createNewAsteroidColBox();
	void beginDraw();
	void endDraw();
	void loadTexture();

	UI* ui;

	//std::vector<Player*> players;
	Player* player1;
	Player* player2;
	std::vector<Asteroid*> asteroids;
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
	sf::RectangleShape player1ColBox;
	sf::RectangleShape player2ColBox;
	std::vector<sf::RectangleShape> asteroidColBoxes;

	int currentWave;
	bool isDebugMode;
	float totalGameTime;
	float networkUpdateTimer = 0.0f;
};