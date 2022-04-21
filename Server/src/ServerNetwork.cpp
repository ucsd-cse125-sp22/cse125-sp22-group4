//#include "stdafx.h" 
#include "ServerNetwork.h"
#include "Network/include/NetworkData.h"
#include <iostream>

ServerNetwork::ServerNetwork(void)
{

    // create WSADATA object
    WSADATA wsaData;

    // our sockets for the server
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;



    // address info for the server to listen to
    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    // set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;    // TCP connection!!!
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Set the mode of the socket to be nonblocking
    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // no longer need address information
    freeaddrinfo(result);

    // start listening for new clients attempting to connect
    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }
}

// accept new connections
bool ServerNetwork::acceptNewClient(unsigned int& id)
{
    // if client waiting, accept the connection and save the socket
    ClientSocket = accept(ListenSocket, NULL, NULL);

    if (ClientSocket != INVALID_SOCKET)
    {
        //disable nagle on the client's socket
        char value = 1;
        setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

        // insert new client into session id table
        //sessions.insert(pair<unsigned int, SOCKET>(id, ClientSocket));

        struct PlayerSession session = {
            id,
            ClientSocket,
            SESSION_CONNECTED,
        };

        sessions.push_back(session);
        return true;
    }

    return false;
}


// receive incoming data
int ServerNetwork::receiveData(PlayerSession& session, char* recvbuf)
{
    iResult = NetworkServices::receiveMessage(session.socket, recvbuf, MAX_PACKET_SIZE);
    if (iResult == 0) {
        session.status = SESSION_DISCONNECTED;
    } else {
        session.status = SESSION_CONNECTED;
    }
    return iResult;
}

int ServerNetwork::sendToSocket(SOCKET socket, char* packets, int totalSize) {
    int iSendResult;
    iSendResult = NetworkServices::sendMessage(socket, packets, totalSize);
    if (iSendResult == SOCKET_ERROR) {
        closesocket(socket);
    }
    return iSendResult;
}

// send data to all clients
void ServerNetwork::sendToAll(char* packets, int totalSize)
{
    int iSendResult;

    for (PlayerSession& session: sessions)
    {
        int result = sendToSocket(session.socket, packets, totalSize);
        if (result == SOCKET_ERROR) {
            session.status = SESSION_DISCONNECTED;
        }
    }
}