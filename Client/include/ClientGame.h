#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <chrono>
#include "ClientNetwork.h"
#include "Client.h"
//#include "Network/include/NetworkData.h"
#include "Network/include/Packets.h"

#define FPS_MAX 1e6/120.0

class ClientGame
{

public:

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendActionPackets(MovementState s);
    void sendRotationPackets(RotationState s);
    void handleSimplePacket(SimplePacket s);

    char network_data[MAX_PACKET_SIZE];

    void update(MovementState s, RotationState r);

    //for debugging move elsewhere later
    void printMat4(glm::mat4 mat);

    void setPlayers(Model** p);

    void updateModels(PlayerState states[PLAYER_NUM]);

    void setItem(glm::mat4 location);
    void setTime(int t);
    void gameEnd(int gameOver, int catWon);
    void setNumPlayers(int p);

    unsigned int getPlayer_id();

private:
    std::chrono::high_resolution_clock timer;
    std::chrono::steady_clock::time_point start_time;
    std::string packet_stream;

    Model** players;
    unsigned int player_id;
    bool loaded;
};