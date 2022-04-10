#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
//#include "Network/include/NetworkData.h"
#include "Network/include/Packets.h"

class ClientGame
{

public:

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets();

    char network_data[MAX_PACKET_SIZE];

    void update();
};