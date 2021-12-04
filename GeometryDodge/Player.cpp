#include "Player.h"
#include <iostream>

Player::Player(int playerNum, sf::RenderWindow* hwnd, Input* in) : playerNum(playerNum), window(hwnd), input(in)
{
	position = sf::Vector2f(0.0f, 0.0f);
	velocity = sf::Vector2f(0.0f, 0.0f);
	speed = 700.0f;
	echoRecvd = false;
	totalGameTime = 0.0f;

	initPlayer();
}

Player::~Player()
{

}

void Player::connect(int playerNumber)
{
	/*int MAXPORT = 49152;
	int MINPORT = 1024;
	int RANGE = MAXPORT - MINPORT;

	int RANDPORT = rand() % RANGE + MINPORT;*/

	conn = new Connection();
	conn->connectToServer(playerNumber);
}

void Player::handleInput(float dt)
{
	// Handle player input
	// Normalise the thumbstick input, the range is [-100, 100]
	normX = input->getLeftStick().x / 100.0f;
	normY = input->getLeftStick().y / 100.0f;
}

void Player::networkUpdate(float dt, int playerID)
{
	sendPlayerData(dt, playerID);
}

void Player::sendPlayerData(float dt, int playerID)
{
	if (playerID == 1)
	{
		// Pack player data
		totalGameTime += dt;

		playerDataSend.playerID = playerID;
		playerDataSend.timeSent = totalGameTime;
		playerDataSend.x = normX;		// Send the normalized input X data
		playerDataSend.y = normY;		// Same for Y

		// Send data to the server
		conn->send(&playerDataSend);
	}
}

void Player::recieveWorldData()
{

}

PlayerDataMsg Player::receiveOtherPlayerData()
{
	PlayerDataMsg msg;

	// All data being received will be game world data and opposite player data
	msg = conn->receive();

	return msg;
}

void Player::localUpdate(float dt)
{
	// Update local player
	playerSprite.move(sf::Vector2f(normX * dt * speed, normY * dt * speed));
	//playerSprite.setPosition(sf::Vector2f(msg.x, msg.y) * dt * speed);
	collisionBox = sf::FloatRect(playerSprite.getPosition().x - size.x * 0.4f, playerSprite.getPosition().y - size.y * 0.4f, size.x * 0.8f, size.y * 0.8f);
	checkScreenBounds();
}

void Player::opponentUpdate(float dt, PlayerDataMsg msg)
{
	// Update other player
	playerSprite.move(sf::Vector2f(msg.x * dt * speed, msg.y * dt * speed));
}

void Player::initPlayer()
{
	loadTexture();
	playerTexture.setSmooth(true);
	playerSprite.setTexture(playerTexture);
	size = playerTexture.getSize();
	playerSprite.setOrigin(size.x * 0.5f, size.y * 0.5f);
	//playerSprite.setOrigin(getTextureRect().width * 0.5f, getTextureRect().height * 0.5f);
	playerSprite.setPosition(sf::Vector2f(200.0f, 200.0f));
	playerSprite.setScale(0.75f, 0.75f);
	collisionBox = sf::FloatRect(playerSprite.getPosition().x - size.x * 0.4f, playerSprite.getPosition().y - size.y * 0.4f, size.x * 0.8f, size.y * 0.8f);
}

void Player::loadTexture()
{
	if (!playerTexture.loadFromFile("res/gfx/ships/altShip_" + std::to_string(playerNum) + ".png"))
	{
		std::cout << "Error loading ship texture " << playerNum << '\n';
	}
}

void Player::checkScreenBounds()
{
	// Check left edge of screen
	if (playerSprite.getPosition().x - (size.x * 0.5f) < 0)
	{
		playerSprite.setPosition(sf::Vector2f(size.x * 0.5f, playerSprite.getPosition().y));
	}

	// Check right edge of screen
	if (playerSprite.getPosition().x + (size.x * 0.5f) > window->getSize().x)
	{
		playerSprite.setPosition(sf::Vector2f(window->getSize().x - size.x * 0.5f, playerSprite.getPosition().y));
	}

	// Check top edge of screen
	if (playerSprite.getPosition().y - (size.y * 0.5f) < 0)
	{
		playerSprite.setPosition(sf::Vector2f(playerSprite.getPosition().x, size.y * 0.5f));
	}

	// Check btm edge of screen
	if (playerSprite.getPosition().y + (size.y * 0.5f) > window->getSize().y)
	{
		playerSprite.setPosition(sf::Vector2f(playerSprite.getPosition().x, window->getSize().y - size.y * 0.5f));
	}
}

sf::Sprite* Player::getPlayerSprite()
{
	return &playerSprite;
}

sf::Vector2u* Player::getPlayerSize()
{
	return &size;
}

sf::FloatRect Player::getCollisionBox()
{
	return collisionBox;
}

void Player::setCollisionBox(float x, float y, float width, float height)
{
	collisionBox = sf::FloatRect(x, y, width, height);
}