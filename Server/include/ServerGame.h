#include <chrono>
#include "ServerNetwork.h"
#include "Network/include/Packets.h"
#include "Constants/include/constants.h"
#include "Graphics/include/PrimitiveMesh.h"
#include "Graphics/include/Maze.h"
#include "Graphics/include/CollisionDetector.h"
#include "Graphics/include/Model.h"
#include "Logic/include/Item.h"
#include "Graphics/include/FakeModel.h"
#include <time.h>
#include <stdlib.h>

// Microseconds / Frames per second
#define FPS_MAX 1e6/60.0

class ServerGame
{

public:

    static bool game_started;
    ServerGame(void);
    ~ServerGame(void);

    void update();
    void collisionStep();

    void receiveFromClients();
    void assignSpawn(int client_id);
    void assignSpawnItem();
    void respawnItem();
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

    Maze* maze;
    Flag* flag;

    // The ServerNetwork object 
    ServerNetwork* network;
    CollisionDetector* collision_detector;

    // data buffer
    char network_data[MAX_PACKET_SIZE];
    PlayerState player_states[PLAYER_NUM];

    //fake models to load in correct OBB
    FakeModel fakePlayerModels[PLAYER_NUM];

    //model of players before handling key stroke
    glm::mat4 oldModels[PLAYER_NUM];

    //model of item locations at spawn
    glm::mat4 oldItemModels[ITEM_NUM_LOC];

    std::chrono::high_resolution_clock timer;
    std::chrono::steady_clock::time_point start_time;

    // for game countdown
    std::chrono::steady_clock timer_t;
    std::chrono::steady_clock::time_point start_t;
    int playTime;

    // these are just for testing respawn
    void isTaken();
    int ans = 0;
};