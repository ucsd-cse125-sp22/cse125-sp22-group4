//#include "stdafx.h" 
#include "ServerGame.h"
#include <iostream>

unsigned int ServerGame::client_id;


void moveGlobal(glm::mat4& model, const glm::vec3& v) {
    model = glm::translate(glm::mat4(1), v) * model;
}


ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork();
    start_time = timer.now();

    maze = new Maze();
    /*
    PlayerState state = player_states[client_id];
    moveGlobal(state.model, glm::vec3(75, 2, -5));
    player_states[client_id] = state;
    */
}

void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n", client_id);
        client_id++;
        PlayerState state = player_states[client_id];
        switch (client_id) {
        case 0:
            //player 1 starting location
            moveGlobal(state.model, glm::vec3(75, 2, -5));
            break;

        case 1:
            // player 2 starting location
            moveGlobal(state.model, glm::vec3(145, 2, -75));
            //tyra->spin(90);
            break;
        case 2:
            // player 3 starting location
            moveGlobal(state.model, glm::vec3(75, 2, -145));
            //tyra->spin(180);
            break;
        case 3:
            // player 4 starting location
            moveGlobal(state.model, glm::vec3(5, 2, -75));
            //tyra->spin(270);
            break;
        }

        // No buffer overflow will happen: acceptNewClient does a check
        player_states[client_id] = state;
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
}

//broadcast game state to all clients
void ServerGame::replicateGameState() {
    const unsigned int packet_size = sizeof(GameStatePacket);
    GameStatePacket packet;
    memcpy(packet.player_states, player_states, sizeof(player_states));
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
void moveLocal(glm::mat4& model, const glm::vec3& v) {
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
    maze->turn = packet->state.turn;
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
        moveLocal(state.model, glm::vec3(0, 0, 0.2));
        break;
    }
    case FORWARD:
    {
       // bool obstacle = maze->isObstacle(client_id, state.model[3][0], state.model[3][2]);
        //if (!obstacle)
            moveLocal(state.model, glm::vec3(0, 0, -0.2));
        break;
    }
    }
    // Actually do the update...
    player_states[client_id] = state;
}