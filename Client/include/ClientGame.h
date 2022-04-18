#pragma once

#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "Client.h"
//#include "Network/include/NetworkData.h"
#include "Network/include/Packets.h"

class ClientGame
{

public:

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets(MovementState s);
    void handleSimplePacket(SimplePacket s);

    char network_data[MAX_PACKET_SIZE];

    void update(MovementState s);

    //for debugging move elsewhere later
    void printMat4(glm::mat4 mat);

    void setPlayers(Model** p);

    void updateModels(PlayerState states[PLAYER_NUM]);

    unsigned int getPlayer_id();

private:

    Model** players;
    unsigned int player_id;
    bool loaded;
};