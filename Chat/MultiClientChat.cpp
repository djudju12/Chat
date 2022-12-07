#include "MultiClientChat.h"
#include <string>


void MultiClientChat::onClientConnected(int clientSocket)
{
	// send welcome message to the connected client
	std::ostringstream ss;
	ss << "Welcome SOCKET #" << clientSocket << "!" << "\n\r";
	std::string welcomeMsg = ss.str();
	send(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

}

void MultiClientChat::onClientDisconnected(int clientSocket)
{

}

void MultiClientChat::onMessageReceived(int clientSocket, const char* msg, int length)
{
	broadcastToClients(clientSocket, msg, length);
}