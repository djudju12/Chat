#pragma once

#include <iostream>
#include <WS2tcpip.h>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")


class TcpListener
{
public:

	TcpListener(const char* IpAddress, int port) :
		m_ipAddress(IpAddress), m_port(port) { }

	// inicializa o listening
	int init();

	// run listening
	int run();

protected:

	virtual void onClientConnected(int clientSocket);

	virtual void onClientDisconnected(int clientSocket);

	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

	void sendToClient(int clientSocket, const char* msg, int length);

	void broadcastToClients(int sendingClient, const char* msg, int length);


private:

	const char*		m_ipAddress;	// IP do endereço que irá rodar o server
	int				m_port;			// Port # para o web service
	int				m_socket;		// FD interno para o listening socket
	fd_set			m_master;			// File description set
};