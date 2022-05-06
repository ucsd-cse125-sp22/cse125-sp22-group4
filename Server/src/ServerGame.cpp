//#include "stdafx.h" 
#include "ServerGame.h"
#include <iostream>

unsigned int ServerGame::client_id;
bool ServerGame::game_started;

void moveGlobal(glm::mat4& model, const glm::vec3& v) {
    model = glm::translate(glm::mat4(1), v) * model;
}

void spin(glm::mat4& model, float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

Model playerModels[PLAYER_NUM];
int flagId;

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork();
    start_time = timer.now();
    maze = new Maze();
    collision_detector = new CollisionDetector();

    // TODO: Should not be hard coded like this.
    player_states[0].modelType = PlayerModelTypes::Dino;
    player_states[1].modelType = PlayerModelTypes::Teapot;
    player_states[2].modelType = PlayerModelTypes::Bunny;
    player_states[3].modelType = PlayerModelTypes::Dino;


    // TOOD: Replace Model overhead... Currently loads in textures
    // but server only needs OBB related code.
    for (int i = 0; i < PLAYER_NUM; ++i) {
        /*
        switch (player_states[i].modelType) {
        case PlayerModelTypes::Teapot:
        {
            playerModels[i] = Model("../../objects/teapot.obj");
            break;
        }
        case PlayerModelTypes::Dino:
        {
            playerModels[i] = Model("../../objects/teapot.obj");
            break;
        }
        case PlayerModelTypes::Bunny:
        {
            playerModels[i] = Model("../../objects/teapot.obj");
            break;
        }
        }
        */
    }
}

void ServerGame::assignSpawn(int client_id) {
    PlayerState& state = player_states[client_id];
    switch (client_id) {
    case 0:
        //player 1 starting location
        moveGlobal(state.model, glm::vec3(75, 0, -5));
        break;
    case 1:
        // player 2 starting location
        moveGlobal(state.model, glm::vec3(145, 0, -75));
        spin(state.model, 90);
        break;
    case 2:
        // player 3 starting location
        moveGlobal(state.model, glm::vec3(75, 0, -145));
        spin(state.model, 180);
        break;
    case 3:
        // player 4 starting location
        moveGlobal(state.model, glm::vec3(5, 0, -75));
        spin(state.model, 270);
        break;
    }
    player_states[client_id] = state;
}

void ServerGame::start() {

    const unsigned int packet_size = sizeof(SimplePacket);
    SimplePacket packet;
    packet.packet_type = GAME_START;
    char* packet_bytes = packet_to_bytes(&packet, packet_size);

    network->sendToAll(packet_bytes, packet_size);
    free(packet_bytes);

    glm::mat4 flagInitLoc = glm::mat4(1);
    moveLocal(flagInitLoc, glm::vec3(5, 0, -5));
    printMat4(flagInitLoc);

    flag = new Flag(flagInitLoc, glm::mat4(1));

    // Move players to spawns
    for (int i = 0; i < PLAYER_NUM; ++i) {
        assignSpawn(i);
        OBB obb{ {1,1},{-1,1}, {-1,-1}, {1,-1} };
        collision_detector->insert(obb);
        printf("insert %d into cd\n", i);
    }
    flagId = collision_detector->insert(flag->getOBB());
    ServerGame::game_started = true;
}

void ServerGame::collisionStep() {

    //collision_detector.
    const OBB bounds{ {1,1},{-1,1}, {-1,-1}, {1,-1} };
    for (int i = 1; i <= PLAYER_NUM; ++i) {
        collision_detector->update(CollisionDetector::computeOBB(bounds, player_states[i-1].model), i-1);
    }

    for (int i = 0; i < PLAYER_NUM; ++i) {
        int hitId = collision_detector->check(i);
        
        // All inserts are in start(), so we know *for now* if it isn't the flag or -1, it's another player
        if (hitId == flagId) {
            printf("[ServerGame::collisionStep] Player %d hit the flag!\n", i);
            flag->item_state.hold = i;
        } else if (hitId >= 0) {
            printf("[ServerGame::collisionStep] Player %d hit player %d!\n", i+1, hitId+1);
        } else {
            //printf("[ServerGame::collisionStep] Player %d has no collisions\n", i);
        }
    }
    printf("\n");
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
        if (!ServerGame::game_started) return;
        collisionStep();
        replicateGameState();
        start_time = timer.now();
    }
}

//broadcast game state to all clients
void ServerGame::replicateGameState() {
    const unsigned int packet_size = sizeof(GameStatePacket);
    GameStatePacket packet;
    memcpy(packet.player_states, player_states, sizeof(player_states));

    packet.item_state = flag->item_state;

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

    bool obstacle = maze->rotateBlock(client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2], (packet->state.rotationalMatrix[2][0] / packet->state.rotationalMatrix[2][2]));
  
    if (!obstacle) {
        state.model = state.model * packet->state.rotationalMatrix;
        player_states[client_id] = state;
    }
    
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
        bool obstacle = maze->leftBlock(client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);

        if (!obstacle)
            moveLocal(state.model, glm::vec3(-0.2, 0, 0));
        break;
    }
    case RIGHT:
    {
        bool obstacle = maze->rightBlock(client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);

        if (!obstacle)
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