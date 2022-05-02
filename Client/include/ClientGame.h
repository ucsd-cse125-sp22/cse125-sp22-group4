#pragma once

#include <string>
#include <winsock2.h>
#include <Windows.h>
#include <string>
#include <fstream>

#include "ClientNetwork.h"
#include "Client.h"
//#include "Network/include/NetworkData.h"
#include "Network/include/Packets.h"
#include "Constants/include/constants.h"

class ClientGame
{

public:

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets(MovementState s);
    void sendRotationPackets(RotationState s);
    void handleSimplePacket(SimplePacket s);
    void handleIDPacket(IDPacket s);

    char network_data[MAX_PACKET_SIZE];

    void update(MovementState s, RotationState r);

    //for debugging move elsewhere later
    void printMat4(glm::mat4 mat);
    bool readCookies();
    void writeCookies();

    void setPlayers(Model** p);
    void updateModels(PlayerState states[PLAYER_NUM]);
    unsigned int getPlayer_id();

private:

    Model** players;
    unsigned int player_id;
    std::string uid;
    bool loaded;
};