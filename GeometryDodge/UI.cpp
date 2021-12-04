#include "UI.h"
#include <iostream>

UI::UI()
{
	waveCount = 1;
	timer = 20;
	count = 0;

	initUI();
}

UI::~UI()
{

}

void UI::initUI()
{
	loadFont();

	// Set up the Wave text
	waveText.setFont(font);
	waveText.setString("WAVE: " + std::to_string(waveCount));
	waveText.setCharacterSize(32);
	waveText.setFillColor(sf::Color::Red);
	waveText.setOutlineColor(sf::Color::Black);
	waveText.setOutlineThickness(1.0f);
	waveText.setPosition(20.0f, 15.0f);

	// Set up the countdown text
	countdownText.setFont(font);
	countdownText.setString("TIME: " + std::to_string(timer));
	countdownText.setCharacterSize(32);
	countdownText.setFillColor(sf::Color::Red);
	countdownText.setOutlineColor(sf::Color::Black);
	countdownText.setOutlineThickness(1.0f);
	countdownText.setPosition(1120.0f, 15.0f);
}

void UI::loadFont()
{
	if (!font.loadFromFile("res/fonts/kenvector_future_thin.ttf"))
	{
		std::cout << "Error loading font\n";
	}
}

void UI::update(float dt)
{
	//timer -= dt;

	count += dt;

	if (count > 1.0f)
	{
		--timer;
		count = 0;

		if (timer == 0)
		{
			timer = 20;
			++waveCount;
		}
	}

	waveText.setString("WAVE: " + std::to_string(waveCount));
	countdownText.setString("TIME: " + std::to_string(timer));
}

void UI::render(sf::RenderWindow* window)
{
	window->draw(waveText);
	window->draw(countdownText);
}

sf::Text* UI::getWaveText()
{
	return &waveText;
}

sf::Text* UI::getCountdownText()
{
	return &countdownText;
}

int UI::getWaveCount()
{
	return waveCount;
}