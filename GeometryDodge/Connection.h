#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
//#include "PlayerDataMsg.h"
#include <queue>
#include "PlayerDataMsg.h"

class Connection
{
public:
	Connection();
	~Connection();

	void connectToServer(int playerNumber);
	
	void send(PlayerDataMsg* msg);
	PlayerDataMsg receive();
	void updatePlayers();

private:
	sf::IpAddress SERVERIP = "127.0.0.1";
	//sf::IpAddress SERVERIP = "192.168.1.188";
	unsigned short SERVERPORT = 5555;
	std::queue<PlayerDataMsg> playerData;
	std::size_t received;
	sf::TcpSocket tcpSocket;
};

