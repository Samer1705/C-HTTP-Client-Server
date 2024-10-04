#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define DEFAULT_PORT "6969"
#define DEFAULT_BUFLEN 512

void vHandleRequest(SOCKET xClientSocket);

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	// Declare and initialize
	WSADATA xWsaData;
	SOCKET xListenSocket = INVALID_SOCKET, xClientSocket = INVALID_SOCKET;
	struct addrinfo *xAddrInfo = NULL, xAddrHints;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &xWsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	printf("Windows socket initialized successfully\n");

	// Determine socket address structure
	ZeroMemory(&xAddrHints, sizeof(xAddrHints));
	xAddrHints.ai_family = AF_INET;
	xAddrHints.ai_socktype = SOCK_STREAM;
	xAddrHints.ai_protocol = IPPROTO_TCP;
	xAddrHints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &xAddrHints, &xAddrInfo);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	printf("Local address & port resolved successfully\n");

	// Create a SOCKET for the server to listen for client connections
	xListenSocket = socket(xAddrInfo->ai_family, xAddrInfo->ai_socktype, xAddrInfo->ai_protocol);
	if (xListenSocket == INVALID_SOCKET)
	{
		printf("socket function failed with error = %d\n", WSAGetLastError());
		freeaddrinfo(xAddrInfo);
		WSACleanup();
		return 1;
	}
	printf("Socket created successfully\n");

	// Setup the TCP listening socket
	iResult = bind(xListenSocket, xAddrInfo->ai_addr, (int)xAddrInfo->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(xAddrInfo);
		closesocket(xListenSocket);
		WSACleanup();
		return 1;
	}
	printf("Socket binded successfully\n");

	freeaddrinfo(xAddrInfo);

	// Listen on IP address and port for incoming connection requests
	if (listen(xListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Listen failed with error: %d\n", WSAGetLastError());
		closesocket(xListenSocket);
		WSACleanup();
		return 1;
	}
	while(1)
	{
		printf("Socket listening...\n");
		// Accept a client socket
		xClientSocket = accept(xListenSocket, NULL, NULL);
		if (xClientSocket == INVALID_SOCKET)
		{
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(xListenSocket);
			WSACleanup();
			return 1;
		}
		printf("Client socket connection accepted\n");


		// shutdown the send half of the connection since no more data will be sent
		iResult = shutdown(xClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(xClientSocket);
			WSACleanup();
			return 1;
		}
	}

	// Cleanup
	closesocket(xClientSocket);
	WSACleanup();
	return 0;
}

void vHandleRequest(SOCKET xClientSocket)
{
	int iSendResult, iRecvBufLen = DEFAULT_BUFLEN, iRecvBytes = 0;
	char cRecvBuf[DEFAULT_BUFLEN];
	iRecvBytes = recv(xClientSocket, cRecvBuf, iRecvBufLen, 0);
	if (iRecvBytes > 0)
	{

	}
}
