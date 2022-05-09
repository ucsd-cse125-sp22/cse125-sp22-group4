//#include "stdafx.h" 
#include "ServerGame.h"
#include <iostream>

unsigned int ServerGame::client_id;

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork();
    start_time = timer.now();
}

void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n", client_id);
        client_id++;
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
    for (PlayerSession& session:network->sessions){
        unsigned int client_id = session.id;
        int data_length = network->receiveData(session, network_data);

        if (data_length <= 0)
            continue;

        int i = 0;
        ushort packet_class = get_packet_class(&(network_data[i]));
        switch (packet_class) {
        case SIMPLE: {
            SimplePacket* pack = (SimplePacket*)malloc(sizeof(SimplePacket));
            memcpy(pack, &network_data[i], sizeof(SimplePacket));
            handleSimplePacket(session, pack);
            i += sizeof(SimplePacket);
            free(pack);
            break;
        }
        case MOVE:
        {
            MovePacket* pack = (MovePacket*)malloc(sizeof(MovePacket));
            memcpy(pack, &network_data[i], sizeof(MovePacket));
            handleMovePacket(session, pack);

            i += sizeof(MovePacket);
            free(pack);

            // TODO: Fix replication... Currently not in lock-step.
            // Note: Moving replication outside of CASE results in dead client.
            //replicateGameState();
            break;
        }
        case ID:
        {
            IDPacket* pack = (IDPacket*)malloc(sizeof(IDPacket));
            memcpy(pack, &network_data[i], sizeof(IDPacket));
            handleIDPacket(session, pack);

            i += sizeof(IDPacket);
            free(pack);
            break;
        }
        case ROTATE:
        {
            RotatePacket* pack = (RotatePacket*)malloc(sizeof(RotatePacket));
            memcpy(pack, &network_data[i], sizeof(RotatePacket));
            handleRotatePacket(session, pack);

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

void ServerGame::handleIDPacket(PlayerSession& session, IDPacket* packet) {
    //for (PlayerSession& s : network->sessions) {
    printf("We are handlign ID packet?");
    printf("%s %d\n", packet->uid, packet->id);
    for (int i = 0; i < network->sessions.size(); ++i) {
        PlayerSession& s = network->sessions[i];
        // Skip if player sessions are same
        if (s.id == session.id)
            continue;

        // If the proposed id and uid are same on packet then
        // replace the old socket with the current session's packet.
        if (s.id == packet->id && s.uid == packet->uid) {
            closesocket(s.socket);
            s.socket = session.socket;
        }
    }
}


void ServerGame::handleSimplePacket(PlayerSession& session, SimplePacket* packet) {
    switch (packet->packet_type) {
    case INIT_CONNECTION:
    {
        IDPacket id_packet;

        // Note: Cast from uint to char (should be safe, assuming < 16 players...)
        id_packet.id = (char)session.id;
        session.uid.copy(id_packet.uid, ID_LEN, 0);
        id_packet.uid[ID_LEN] = '\0';
        //std::strncpy(id_packet.uid, session.uid.c_str(), ID_LEN);
        //id_packet.uid = session.uid.c_str();
        char* packet_bytes = packet_to_bytes(&id_packet, sizeof(id_packet));
        network->sendToSocket(session.socket, packet_bytes, sizeof(id_packet));
    }
    }
}

//multiply the rotational matrix from client to the actual model
void ServerGame::handleRotatePacket(PlayerSession& session, RotatePacket* packet) {
    unsigned int client_id = session.id;
    PlayerState state = player_states[client_id];
    if (!state.alive) {
        return;
    }
    state.model = state.model * packet->state.rotationalMatrix;
    player_states[client_id] = state;
}

//Update player_state from move packet.
void ServerGame::handleMovePacket(PlayerSession& session, MovePacket* packet) {
    unsigned int client_id = session.id;
    PlayerState state = player_states[client_id];
    if (!state.alive) {
        return;
    }

    switch (packet->state.dir) {
    case LEFT:
    {
        GraphicsUtil::moveLocal(state.model, glm::vec3(-0.2, 0, 0));
        break;
    }
    case RIGHT:
    {
        GraphicsUtil::moveLocal(state.model, glm::vec3(0.2, 0, 0));
        break;
    }
    case BACK:
    {
        GraphicsUtil::moveLocal(state.model, glm::vec3(0, 0, 0.2));
        break;
    }
    case FORWARD:
    {
        GraphicsUtil::moveLocal(state.model, glm::vec3(0, 0, -0.2));
        break;
    }
    }
    // Actually do the update...
    player_states[client_id] = state;
}