#include <winsock2.h>
#include <Windows.h>
#include "Network/include/NetworkServices.h"
#include "Constants/include/constants.h"
#include <ws2tcpip.h>
#include <map>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "2333" 

class ServerNetwork
{

public:

    ServerNetwork(void);
    ~ServerNetwork(void);

    // Socket to listen for new connections
    SOCKET ListenSocket;

    // Socket to give to the clients
    SOCKET ClientSocket;

    // for error checking return values
    int iResult;

    // table to keep track of each client's socket
    std::map<unsigned int, SOCKET> sessions;

    // accept new connections
    bool acceptNewClient(unsigned int& id);

    // receive incoming data
    int receiveData(unsigned int client_id, char* recvbuf);

    int sendToSocket(SOCKET, char* packets, int totalSize);
    // send data to all clients
    void sendToAll(char* packets, int totalSize);
};