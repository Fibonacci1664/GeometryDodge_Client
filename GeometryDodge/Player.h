#pragma once
#include <SFML/Graphics.hpp>
#include "Input.h"
#include "Connection.h"
#include "PlayerDataMsg.h"

class Player
{
public:
	Player() {}
	Player(int playerNum, sf::RenderWindow* hwnd, Input* in);
	~Player();

	void connect(int playerNumber);
	void handleInput(float dt);
	void networkUpdate(float dt, int playerID);
	void localUpdate(float dt);
	void opponentUpdate(float dt, PlayerDataMsg msg);
	void sendPlayerData(float dt, int playerID);
	void recieveWorldData();
	PlayerDataMsg receiveOtherPlayerData();
	
	sf::Sprite* getPlayerSprite();
	sf::Vector2u* getPlayerSize();
	sf::FloatRect getCollisionBox();
	void setCollisionBox(float x, float y, float width, float height);
	
private:
	
	void initPlayer();
	void loadTexture();
	void checkScreenBounds();

	Input* input;
	sf::RenderWindow* window;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2u size;
	sf::FloatRect collisionBox;
	Connection* conn;
	PlayerDataMsg playerDataSend;
	PlayerDataMsg playerDataRecv;

	unsigned short SERVERPORT = 5555;
	float totalGameTime;
	int playerNum;
	float speed;
	bool echoRecvd;
	float normX;
	float normY;
};