#include <chrono>
#include "ServerNetwork.h"
#include "Network/include/Packets.h"
#include "Constants/include/constants.h"
#include "Graphics/include/PrimitiveMesh.h"
#include "Graphics/include/Maze.h"

// Microseconds / Frames per second
#define FPS_MAX 1e6/75.0

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

    void update();
    void receiveFromClients();
    void handleMovePacket(int client_id, MovePacket* s);
    void handleSimplePacket(int client_id, SimplePacket* s);
    void handleRotatePacket(int client_id, RotatePacket* s);
    void replicateGameState();

private:
    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

    Maze* maze;

    // The ServerNetwork object 
    ServerNetwork* network;

    // data buffer
    char network_data[MAX_PACKET_SIZE];
    PlayerState player_states[PLAYER_NUM];

    std::chrono::high_resolution_clock timer;
    std::chrono::steady_clock::time_point start_time;
};