#include "TcpListener.h"

int TcpListener::init()
{
	// Inicializar winsock
	// contém informações sobre a implementação do winsock
	WSAData data;
	// ver winsock 2.2
	WORD ver = MAKEWORD(2, 2);
	// Inicializa. Se não for possível, retorna
	int wsOk = WSAStartup(ver, &data);

	if (wsOk != 0)
		return wsOk;

	// Criar o socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
		return WSAGetLastError();

	// Bind o socket em um endereço de ip e porta
	// https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress, &hint.sin_addr);

	// no link do comentário anterior o cast do hint é explicado
	if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
		return WSAGetLastError();
	

	// avisar o winsock que o socket é para "listening"
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
		return WSAGetLastError();

	FD_ZERO(&m_master);
	FD_SET(m_socket, &m_master);

	return 0;
}

int TcpListener::run() 
{

	bool running = true;
	while (running)
	{
		fd_set copyMaster = m_master;

		int socketCount = select(0, &copyMaster, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++)
		{
			SOCKET sock = copyMaster.fd_array[i];
			if (sock == m_socket)
			{
				// Accept new connection
				SOCKET client = accept(m_socket, nullptr, nullptr);

				FD_SET(client, &m_master);

				onClientConnected(client);
			}
			else
			{
				char buf[4096];
				ZeroMemory(buf, 4096);
				// Accept a new message
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived <= 0)
				{
					// drop the client
					onClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &m_master);
				}
				else
				{
					onMessageReceived(sock, buf, bytesReceived);
				}
			}
		}
	}

	FD_CLR(m_socket, &m_master);
	closesocket(m_socket);

	while (m_master.fd_count > 0)
	{
		SOCKET sock = m_master.fd_array[0];
		FD_CLR(sock, &m_master);
		closesocket(sock);
	}


	// Desligar winson
	WSACleanup();
	return 0;
}

void TcpListener::sendToClient(int clientSocket, const char* msg, int length)
{
	send(clientSocket, msg, length, 0);
}

void TcpListener::broadcastToClients(int sendingClient, const char* msg, int length)
{

	for (int i = 0; i < m_master.fd_count; i++)
	{
		SOCKET outSock = m_master.fd_array[i];

		if (outSock != m_socket && outSock != sendingClient)
			sendToClient(outSock, msg, length);
	}
}

void TcpListener::onMessageReceived(int clientSocket, const char* msg, int length){}

void TcpListener::onClientConnected(int clientSocket){}

void TcpListener::onClientDisconnected(int clientSocket){}
