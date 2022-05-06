//#include "stdafx.h" 
#include "ServerGame.h"
#include <iostream>

unsigned int ServerGame::client_id;


void moveGlobal(glm::mat4& model, const glm::vec3& v) {
    model = glm::translate(glm::mat4(1), v) * model;
}

void spin(glm::mat4& model, float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}


ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork();
    start_time = timer.now();
    maze = new Maze();
    playTime = 0;

    // I have no idea what the cooldown time is...I think 5 seconds was proposed
    cooldownTime = 5;
}

void ServerGame::assignSpawn(int client_id) {
    PlayerState& state = player_states[client_id];
    switch (client_id) {
    case 0:
        //player 1 starting location
        moveGlobal(state.model, glm::vec3(75, 2, -5));
        break;
    case 1:
        // player 2 starting location
        moveGlobal(state.model, glm::vec3(145, 2, -75));
        spin(state.model, 90);
        break;
    case 2:
        // player 3 starting location
        moveGlobal(state.model, glm::vec3(75, 2, -145));
        spin(state.model, 180);
        break;
    case 3:
        // player 4 starting location
        moveGlobal(state.model, glm::vec3(5, 2, -75));
        spin(state.model, 270);
        break;
    }
    player_states[client_id] = state;
}

void ServerGame::assignSpawnItem() {

    glm::mat4 flagInitLoc = glm::mat4(1);

    int random = rand() % 5;
    printf("%d spawn\n", random);

    // choose random spawn location
    switch (random) {
    case 0:
        moveGlobal(flagInitLoc, glm::vec3(145, 1, -25));
        break;
    case 1:
        moveGlobal(flagInitLoc, glm::vec3(125, 1, -145));
        break;
    case 2:
        moveGlobal(flagInitLoc, glm::vec3(5, 1, -5));
        break;
    case 3:
        moveGlobal(flagInitLoc, glm::vec3(5, 1, -145));
        break;
    case 4:
        moveGlobal(flagInitLoc, glm::vec3(96, 1, -53));
        break;
    }

    flag = new Flag(flagInitLoc, glm::mat4(1));
    flag->randomSpawn = random; // remember new location
}

void ServerGame::respawnItem(Flag* flag) {
    int random = flag->randomSpawn;

    // get new respawn location
    while (random == flag->randomSpawn) {
        random = rand() % 5;
    }
    //printf("%d random\n", random);
    glm::mat4 model = flag->item_state.model;

    // choose random respawn location
    switch (random) {
    case 0:
        moveGlobal(model, glm::vec3(145, 1, -25));
        break;
    case 1:
        moveGlobal(model, glm::vec3(125, 1, -145));
        break;
    case 2:
        moveGlobal(model, glm::vec3(5, 1, -5));
        break;
    case 3:
        moveGlobal(model, glm::vec3(5, 1, -145));
        break;
    case 4:
        moveGlobal(model, glm::vec3(96, 1, -53));
        break;
    }

    flag->item_state.model = model;

}

void ServerGame::start() {
    const unsigned int packet_size = sizeof(SimplePacket);
    SimplePacket packet;
    packet.packet_type = GAME_START;
    char* packet_bytes = packet_to_bytes(&packet, packet_size);

    network->sendToAll(packet_bytes, packet_size);
    free(packet_bytes);

    // start game time TODO wait for all four players
    start_t = timer_t.now();

    // Move item to spawn
    assignSpawnItem();

    // Move players to spawns
    for (int i = 0; i <= client_id; ++i) {
        assignSpawn(i);
    }
}

void ServerGame::update()
{
  
    // get new clients
    if (network->acceptNewClient(client_id))
    {
        client_id++;
        printf("client %d has been connected to the server\n", client_id);
        if (1) {
            // Send game start packets?
            printf("Game start\n");
            start();
        }
    }
    // Receive from clients as fast as possible.
    receiveFromClients();

    // Calculate tick
    auto stop_time = timer.now();
    auto dt = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    if (dt.count() >= FPS_MAX) {
        replicateGameState();
        start_time = timer.now();
    }
   
    auto stop_t = timer_t.now();
    auto test = std::chrono::duration_cast<std::chrono::seconds>(stop_t - start_t);
    
    playTime = test.count();

    //if (!cooldown.empty()) {
    //    for (int i = 0; i < cooldown.size(); i++) {

    //    }
    //}
   

        
}

//broadcast game state to all clients
void ServerGame::replicateGameState() {
    const unsigned int packet_size = sizeof(GameStatePacket);
    GameStatePacket packet;
    memcpy(packet.player_states, player_states, sizeof(player_states));

    packet.item_state = flag->item_state;
    packet.game.gameTime = playTime;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);

    network->sendToAll(packet_bytes, packet_size);
    free(packet_bytes);
}

void ServerGame::receiveFromClients()
{

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        unsigned int client_id = iter->first;
        int data_length = network->receiveData(client_id, network_data);

        if (data_length <= 0)
            continue;

        int i = 0;
        ushort packet_class = get_packet_class(&(network_data[i]));
        switch (packet_class) {
        case SIMPLE: {
            SimplePacket* pack = (SimplePacket*)malloc(sizeof(SimplePacket));
            memcpy(pack, &network_data[i], sizeof(SimplePacket));
            handleSimplePacket(client_id, pack);
            i += sizeof(SimplePacket);
            free(pack);
            break;
        }
        case MOVE:
        {
            MovePacket* pack = (MovePacket*)malloc(sizeof(MovePacket));
            memcpy(pack, &network_data[i], sizeof(MovePacket));
            handleMovePacket(client_id, pack);

            i += sizeof(MovePacket);
            free(pack);

            // TODO: Fix replication... Currently not in lock-step.
            // Note: Moving replication outside of CASE results in dead client.
            //replicateGameState();
            break;
        }
        case ROTATE:
        {
            RotatePacket* pack = (RotatePacket*)malloc(sizeof(RotatePacket));
            memcpy(pack, &network_data[i], sizeof(RotatePacket));
            handleRotatePacket(client_id, pack);

            i += sizeof(RotatePacket);
            free(pack);

            break;
        }
        default:
            printf("error in packet types\n");
            break;
        }
    }
    // Replicate game state when everything is processed in this frame.
}

//method to translate the model matrix
// TODO: Make use of graphics library instead. Have an object wrap the player's positions
// and use methods to manipulate.
void ServerGame::moveLocal(glm::mat4& model, const glm::vec3& v) {
    model = model * glm::translate(glm::mat4(1), v);
}


void ServerGame::handleSimplePacket(int client_id, SimplePacket* packet) {
    switch (packet->packet_type) {
    case INIT_CONNECTION:
    {
		std::map<unsigned int, SOCKET>::iterator iter = network->sessions.find(client_id);
		if (iter != network->sessions.end()) {
			SOCKET player_socket = iter->second;
            SimplePacket id_packet;
            id_packet.packet_type = INIT_CONNECTION;

            // Note: Cast from uint to char (should be safe, assuming < 16 players...)
            id_packet.data = (char)iter->first;
            char* packet_bytes = packet_to_bytes(&id_packet, sizeof(id_packet));
            network->sendToSocket(player_socket, packet_bytes, sizeof(id_packet));
		}
    }
    }
}

//multiply the rotational matrix from client to the actual model
void ServerGame::handleRotatePacket(int client_id, RotatePacket* packet) {
    PlayerState state = player_states[client_id];
    if (!state.alive) {
        return;
    }
    state.model = state.model * packet->state.rotationalMatrix;
    player_states[client_id] = state;
}

//Update player_state from move packet.
void ServerGame::handleMovePacket(int client_id, MovePacket* packet) {
    PlayerState state = player_states[client_id];
    if (!state.alive) {
        return;
    }

    switch (packet->state.dir) {
    case LEFT:
    {
        moveLocal(state.model, glm::vec3(-0.2, 0, 0));
        break;
    }
    case RIGHT:
    {
        moveLocal(state.model, glm::vec3(0.2, 0, 0));
        break;
    }
    case BACK:
    {
        bool obstacle = maze->backwardsBlock(client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);

        if (!obstacle)
            moveLocal(state.model, glm::vec3(0, 0, 0.2));
        break;
    }
    case FORWARD:
    {
        bool obstacle = maze->forwardBlock(client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);

        if (!obstacle) {
            moveLocal(state.model, glm::vec3(0, 0, -0.2));
        }

        break;
    }
    }
    // Actually do the update...
    player_states[client_id] = state;
}

void ServerGame::printMat4(glm::mat4 mat) {
    printf("%f, %f, %f, %f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    printf("%f, %f, %f, %f\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    printf("%f, %f, %f, %f\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    printf("%f, %f, %f, %f\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}