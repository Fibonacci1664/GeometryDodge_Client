#include "Connection.h"

Connection::Connection()
{
    
}

Connection::~Connection()
{

}

void Connection::connectToServer(int playerNumber)
{
    // Create TCP  
    sf::Time timeout = sf::seconds(10.0f);
    sf::Socket::Status socketStatus = tcpSocket.connect(SERVERIP, SERVERPORT, timeout);

    if (socketStatus != sf::Socket::Done)
    {
        std::cout << "Not connected!\n";
    }
    else
    {
        std::cout << "Player: " << playerNumber << " has connected to server port number : " << SERVERPORT
            << "\nFrom local port number: " << tcpSocket.getLocalPort()
            << "\nAnd Server IP number: " << tcpSocket.getRemoteAddress() << '\n';
    }
}

void Connection::updatePlayers()
{
	// Check for incoming data
	// Send player data (1->2) (2->1) use id's
	// Send asteroid data to all players
}

void Connection::send(PlayerDataMsg* msg)
{
    //tcpSocket.setBlocking(false);

    // Send data
    if (tcpSocket.send(msg, sizeof(PlayerDataMsg)) != sf::Socket::Done)
    {
        std::cout << "Error sending data by TCP!\n";
        return;
    }

    std::cout << "Player data sent\n";

    std::cout << "Player ID: " << msg->playerID << '\n';
    std::cout << "Msg time sent: " << msg->timeSent << '\n';
    std::cout << "Player X: " << msg->x << '\n';
    std::cout << "Player Y: " << msg->y << '\n';
}

PlayerDataMsg Connection::receive()
{
	//playerData.push(msg);
    PlayerDataMsg msgRecv;

    //tcpSocket.setBlocking(false);

    std::cout << "About to recv\n";

	if (tcpSocket.receive(&msgRecv, sizeof(PlayerDataMsg), received) != sf::Socket::Done)
	{
		std::cout << "Client side error receiving using TCP!\n";
        return msgRecv;
	}

    std::cout << "Client side received echo: " << received << " bytes\n";

    std::cout << "Player ID: " << msgRecv.playerID << '\n';
    std::cout << "Msg time sent: " << msgRecv.timeSent << '\n';
    std::cout << "Player X: " << msgRecv.x << '\n';
    std::cout << "Player Y: " << msgRecv.y << '\n';

    return msgRecv;
}
