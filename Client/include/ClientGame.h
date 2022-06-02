#pragma once

#include <winsock2.h>
#include <Windows.h>
#include <chrono>
#include <array>
#include "ClientNetwork.h"
#include "Client.h"
//#include "Network/include/NetworkData.h"
#include "Network/include/Packets.h"

#define FPS_MAX 1e6/120.0
#define END_SCREEN_TIME 8

class ClientGame
{

public:

    ClientGame();
    ~ClientGame(void);

    ClientNetwork* network;

    void sendHideStart();

    void sendGameStart();
    void sendPlayerSelect(int choice);
    void sendPairCount(int pair1, int pair2);
    std::array<int, PLAYER_NUM> getPlayerSelection();

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
    void setFinalDest(glm::mat4 location, int f);

    unsigned int getPlayer_id();

private:
    std::chrono::high_resolution_clock timer;
    std::chrono::steady_clock::time_point start_time;
    std::string packet_stream;

    std::chrono::steady_clock::time_point time_ended;

    int playerSelection[PLAYER_NUM];

    Model** players;
    unsigned int player_id;
    bool loaded;
    bool game_ended;
};