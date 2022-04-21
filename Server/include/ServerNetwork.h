#include <winsock2.h>
#include <Windows.h>
#include "Network/include/NetworkServices.h"
#include <ws2tcpip.h>
#include <map>
#include <vector>

using namespace std;
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"

enum SessionStatus {
    SESSION_CONNECTED,
    SESSION_DISCONNECTED,
    SESSION_NA,
};

struct PlayerSession {
    unsigned int id;
    SOCKET socket;
    SessionStatus status = SESSION_NA;
};

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
    //std::map<unsigned int, SOCKET> sessions;
    std::vector<PlayerSession> sessions;

    // accept new connections
    bool acceptNewClient(unsigned int& id);

    // receive incoming data
    int receiveData(PlayerSession& session, char* recvbuf);

    int sendToSocket(SOCKET, char* packets, int totalSize);
    // send data to all clients
    void sendToAll(char* packets, int totalSize);
};