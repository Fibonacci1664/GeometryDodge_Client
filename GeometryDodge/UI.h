#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class UI
{
public:
	UI();
	~UI();

	void update(float dt);
	void render(sf::RenderWindow* window);
	sf::Text* getWaveText();
	sf::Text* getCountdownText();
	int getWaveCount();

private:
	void initUI();
	void loadFont();

	sf::Font font;
	sf::Text waveText;
	sf::Text countdownText;

	int waveCount;
	int timer;
	float count;
};