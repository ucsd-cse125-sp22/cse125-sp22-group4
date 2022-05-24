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
#include <queue>
#include <unordered_map>

// Microseconds / Frames per second
#define FPS_MAX 1e6/60.0
#define PLAYER_DEFAULT_SPEED 0.2

#define CONFIG_FILE "../../config.yaml"
#define DEFAULT_CATSPEED 0.4
#define DEFAULT_MOUSESPEED 0.4
#define DEFAULT_ROUNDLENGTHSEC 180
#define DEFAULT_COOLDOWNTIMESEC 5
#define DEFAULT_CATVIEWITEMSEC 5
#define DEFAULT_PLAYER0DEVMODE false

#define CAT_ID 0

// W,S,A,D
const glm::vec3 DIR_TO_VEC[4] = {
   glm::vec3(0, 0, -1),
   glm::vec3(0, 0, 1),
   glm::vec3(-1, 0, 0),
   glm::vec3(1, 0, 0)
};

class ServerGame
{

public:

    static bool game_started;
    ServerGame();
    ~ServerGame(void);

    void update();
    void collisionStep();
    void receiveFromClients();

    void setupStationaryObjective();
    void checkStationaryObjective();

    void assignSpawn(int client_id);
    void assignSpawnItem();
    void assignSpawnItem2();
    void respawnItem();
    void respawnItem2();
    void respawnPlayer(int client_id);
    void spawnFinalDestination();
    void respawnFinalDest();
    void start();
    void handleMovePacket(int client_id, MovePacket* s);
    void handleSimplePacket(int client_id, SimplePacket* s);
    void handleRotatePacket(int client_id, RotatePacket* s);

    void replicateGameState();
    void announceGameEnd(bool winner);

    void moveLocal(glm::mat4& model, const glm::vec3& v);
    void printMat4(glm::mat4 mat);

    void updateFromConfigFile();

private:
    // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;
    bool gameAlive;
    
    // config file fields
    double catSpeed;
    double mouseSpeed;
    int ticksSinceConfigCheck;
    int roundLengthSec;
    int cooldownTimeSec;
    bool player0DevMode; // allow player 0 to kill mice, hold item, and see everything on the minimap
    int catViewItemSec; // time in seconds cat can view item in minimap

    Maze* maze;
    Flag* flag;
    SitAndHoldObjective* stationary;

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

    //model of item and player locations at spawn
    glm::mat4 oldItemPositions[ITEM_NUM_LOC];
    glm::mat4 oldItem2Positions[ITEM_NUM_LOC];
    glm::mat4 oldPlayerPositions[PLAYER_NUM];
    glm::mat4 oldFinalDestinations[4];

    //player location for dead mice
    glm::mat4 banished;

    std::chrono::high_resolution_clock timer;
    std::chrono::steady_clock::time_point start_time;

    // for game countdown
    std::chrono::steady_clock timer_t;
    std::chrono::steady_clock::time_point start_t;
    int playTime;

    // for dead mouse cooldown
    std::queue< std::pair<int, std::chrono::steady_clock::time_point> > cooldown; // player ID and start time
    std::chrono::steady_clock timer_mouse;
    std::chrono::steady_clock::time_point lastMouseDeath;
    void mouseDead(int client_id);
    void checkCooldownOver();

    // For collision detection
    Model playerModels[PLAYER_NUM];
    int flagId;
    int bearId;
    int stationaryId;

    // these are just for testing respawn
    void isTaken();
    int ans = 0;

    glm::mat4 destModel; // where item is taken to

    // for limited cat view of item in minimap
    bool catViewItem = false;

    // for final destination rotation
    std::chrono::steady_clock timer_finalDest;
    std::chrono::steady_clock::time_point start_finalDest;
    bool flag_taken = false;
    void checkFinalDestRotates();
    int finalDestTime = -1; // how much time has elapsed
    int finalDestRotatesTime = 45; // how long it takes for final dest to rotate
    int finalDestLoc = -1;
};