#include "Level.h"

Level::Level(sf::RenderWindow* hwnd, Input* controller_0, GameState* gs) :
	Screen(hwnd, controller_0, gs)
{
	isDebugMode = true;
	totalGameTime = 0.0f;

	initLevel();

	if (isDebugMode)
	{
		initDebugMode();
	}
}

Level::~Level()
{
	if (player1)
	{
		delete player1;
		player1 = nullptr;
	}

	if (player2)
	{
		delete player2;
		player2 = nullptr;
	}

	if (ui)
	{
		delete ui;
		ui = nullptr;
	}

	for (int i = 0; i < asteroids.size(); ++i)
	{
		if (asteroids[i])
		{
			delete asteroids[i];
			asteroids[i] = nullptr;
		}
	}
}

void Level::initLevel()
{
	initBackground();
	initUI();
	initPlayer();
	initAsteroids();
}

void Level::initDebugMode()
{
	// Player debug collision box
	player1ColBox.setFillColor(sf::Color(0, 0, 0, 0));
	player1ColBox.setOutlineColor(sf::Color::Red);
	player1ColBox.setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	player1ColBox.setPosition(sf::Vector2f(player1->getCollisionBox().left, player1->getCollisionBox().top));
	player1ColBox.setSize(sf::Vector2f(player1->getCollisionBox().width, player1->getCollisionBox().height));

	if (player2)
	{
		player2ColBox.setFillColor(sf::Color(0, 0, 0, 0));
		player2ColBox.setOutlineColor(sf::Color::Yellow);
		player2ColBox.setOutlineThickness(1.0f);

		player2ColBox.setPosition(sf::Vector2f(player2->getCollisionBox().left, player2->getCollisionBox().top));
		player2ColBox.setSize(sf::Vector2f(player2->getCollisionBox().width, player2->getCollisionBox().height));
	}

	// Create a new asteroid collision box
	asteroidColBoxes.push_back(sf::RectangleShape());

	// Asteroids debug collision boxes
	asteroidColBoxes[0].setFillColor(sf::Color(0, 0, 0, 0));
	asteroidColBoxes[0].setOutlineColor(sf::Color::Magenta);
	asteroidColBoxes[0].setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	asteroidColBoxes[0].setPosition(sf::Vector2f(asteroids[0]->getCollisionBox().left, asteroids[0]->getCollisionBox().top));
	asteroidColBoxes[0].setSize(sf::Vector2f(asteroids[0]->getCollisionBox().width, asteroids[0]->getCollisionBox().height));
}

void Level::initBackground()
{
	loadTexture();
	bgTexture.setSmooth(true);
	bgSprite.setTexture(bgTexture);
	//bgSprite.setOrigin(bgSprite.getTextureRect().width * 0.5f, bgSprite.getTextureRect().height * 0.5f);
	////bgSprite.setPosition(sf::Vector2f(0.0f, 0.0f));
	bgSprite.setScale(5.0f, 2.8125f);
}

void Level::initUI()
{
	ui = new UI;
	currentWave = ui->getWaveCount();
}

void Level::initPlayer()
{
	player1 = new Player(1, window, controller_0);
	player1->connect(1);

	// This second player is purely from rendering on a clients local game
	// It does not matter that a controller obj has been passed to this player
	// Nor that it is the same controller obj as player 1, as player 2 does not get controlled
	// but is only updated and rendered from the information passed from the server
	// and by extension the other player. This means that both players are player 1 on their own local machines
	//player2 = new Player(2, window, controller_0);
	//player2->getPlayerSprite()->setPosition(300.0f, 300.0f);
	//player2->connect(2);
}

void Level::initAsteroids()
{
	asteroids.push_back(new Asteroid(window));
}

void Level::spawnNewAsteroid()
{
	// If we get here it means the wave count was incremented, so spawn an asteroid
	asteroids.push_back(new Asteroid(window));
	createNewAsteroidColBox();
}

void Level::checkCurrentWave()
{
	if (currentWave != ui->getWaveCount())
	{
		spawnNewAsteroid();
		currentWave = ui->getWaveCount();
	}
}

void Level::handleInput(float dt)
{
	// Handle input of local player only
	player1->handleInput(dt);
	//player2->handleInput(dt);
}

void Level::update(float dt)
{
	totalGameTime += dt;
	//networkUpdateTimer += dt;

	ui->update(dt);

	std::vector<GameWorldData> gameWorldData;

	// Do network update, for player 1 this means sending own data to server for client 2 to use(player 2 functionality not built yet)
	//player1->networkUpdate(dt, 1);
	gameWorldData.push_back(player1->networkUpdate(dt, 1));
	//networkUpdateTimer = 0.0f;


	// Update actual player with local input information and game world data received from the network
	player1->localUpdate(dt);

	// If player 2 exists update them
	//if (player2)
	//{
	//	// Do network update, for player 2 this means recv positinal data of other player
	//	PlayerDataMsg msg2 = player2->receiveOtherPlayerData();

	//	// Update player 2 sprite with positional data received from server and game world data
	//	player2->opponentUpdate(dt, msg2);
	//}
	
	if (isDebugMode)
	{
		// Update the players collision box
		player1ColBox.setPosition(sf::Vector2f(player1->getCollisionBox().left, player1->getCollisionBox().top));

		if (player2)
		{
			player2ColBox.setPosition(sf::Vector2f(player2->getCollisionBox().left, player2->getCollisionBox().top));
		}

		// Update all the asteroids collision boxes
		for (int i = 0; i < asteroidColBoxes.size(); ++i)
		{
			asteroidColBoxes[i].setPosition(sf::Vector2f(asteroids[i]->getCollisionBox().left, asteroids[i]->getCollisionBox().top));
		}
	}

	checkCurrentWave();

	// Will also need a struct containing data for the world objects, asteroid, UI counters etc.
	// i.e WorldDataMsg worldMsg, then we use this data to update the local asteroid and game timers

	// If game world data exists, i.e. if we have carried out a network update
	if (!gameWorldData.empty())
	{
		// Update all the asteroids using the gameWorld data from the network update above
		for (int i = 0; i < asteroids.size(); i++)
		{
			asteroids[i]->update(dt, gameWorldData[i]);
		}
	}
	
	//checkCollisions();
}

void Level::render()
{
	beginDraw();

	// Render stuff here
	window->draw(bgSprite);
	window->draw(*ui->getWaveText());
	window->draw(*ui->getCountdownText());

	// Draw all the asteroids
	for (int i = 0; i < asteroids.size(); i++)
	{
		window->draw(*asteroids[i]->getAsteroidSprite());
	}

	window->draw(*player1->getPlayerSprite());

	// If player 2 exists render them
	if (player2)
	{
		window->draw(*player2->getPlayerSprite());
	}

	// Draw all the debug magenta collision boxes
	if (isDebugMode)
	{
		window->draw(player1ColBox);
		window->draw(player2ColBox);

		for (int i = 0; i < asteroidColBoxes.size(); ++i)
		{
			window->draw(asteroidColBoxes[i]);
		}
	}

	endDraw();
}

void Level::beginDraw()
{
	window->clear(sf::Color::Black);
}

void Level::endDraw()
{
	window->display();
}

void Level::loadTexture()
{
	if (!bgTexture.loadFromFile("res/gfx/bg/blue.png"))
	{
		std::cout << "Error loading background texture.\n";
	}
}

void Level::checkCollisions()
{
	// If player collides with an asteroid, set GAME_OVER state
	for (int i = 0; i < asteroids.size(); ++i)
	{
		if (player1->getCollisionBox().intersects(asteroids[i]->getCollisionBox()))
		{
			gameState->setCurrentState(State::GAMEOVER);
		}
	}
}

void Level::createNewAsteroidColBox()
{
	// Create a new asteroid collision box
	asteroidColBoxes.push_back(sf::RectangleShape());

	// Access the last element in the vector, i.e. the one we just pushed back
	int index = asteroidColBoxes.size() - 1;

	asteroidColBoxes[index].setFillColor(sf::Color(0, 0, 0, 0));
	asteroidColBoxes[index].setOutlineColor(sf::Color::Magenta);
	asteroidColBoxes[index].setOutlineThickness(1.0f);

	// Set the collision box origin to be the top left of the sprites local coords, this is
	// based on the already defined collision box which was defined during object creation
	asteroidColBoxes[index].setPosition(sf::Vector2f(asteroids[index]->getCollisionBox().left, asteroids[index]->getCollisionBox().top));
	asteroidColBoxes[index].setSize(sf::Vector2f(asteroids[index]->getCollisionBox().width, asteroids[index]->getCollisionBox().height));
}