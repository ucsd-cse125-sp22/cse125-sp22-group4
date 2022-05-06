#include <chrono>
#include "ServerNetwork.h"
#include "Network/include/Packets.h"
#include "Constants/include/constants.h"
#include "Graphics/include/PrimitiveMesh.h"
#include "Graphics/include/Maze.h"
#include "Logic/include/Item.h"
#include <queue>

// Microseconds / Frames per second
#define FPS_MAX 1e6/60.0

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

    void update();
    void receiveFromClients();
    void assignSpawn(int client_id);
    void assignSpawnItem();
    void respawnItem(Flag* flag);
    void start();
    void handleMovePacket(int client_id, MovePacket* s);
    void handleSimplePacket(int client_id, SimplePacket* s);
    void handleRotatePacket(int client_id, RotatePacket* s);
    void replicateGameState();

    void moveLocal(glm::mat4& model, const glm::vec3& v);
    void printMat4(glm::mat4 mat);

private:
    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;
    static bool game_started;

    Maze* maze;
    Flag* flag;

    // The ServerNetwork object 
    ServerNetwork* network;

    // data buffer
    char network_data[MAX_PACKET_SIZE];
    PlayerState player_states[PLAYER_NUM];

    std::chrono::high_resolution_clock timer;
    std::chrono::steady_clock::time_point start_time;

    // for game countdown
    std::chrono::steady_clock timer_t;
    std::chrono::steady_clock::time_point start_t;
    int playTime;

    // for dead mouse cooldown
    int cooldownTime;
    std::queue< std::pair<int, int> > cooldown; // player ID and microseconds remaining
    std::chrono::steady_clock timer_mouse;
    std::chrono::steady_clock::time_point start_mouse;
    void mouseDead(int client_id);
};
