#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include "GameState.h"
#include "Level.h"

class Application
{
public:
	Application(int width, int height);
	~Application();

	void run();

private:
	void initWindow();
	void processWindowEvents();
	void runGameLoop(Level* level, float deltaTime);

	sf::RenderWindow window;
	Input controller_0;
	Input controller_1;
	GameState gameState;
	Level* level;

	int windowWidth;
	int windowHeight;

};