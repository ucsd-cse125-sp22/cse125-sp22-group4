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
}

void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n", client_id);

        client_id++;
    }
    receiveFromClients();
}

void ServerGame::receiveFromClients()
{

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0)
        {
            //no data recieved
            continue;
        }

        int i = 0;
        ushort packet_class = get_packet_class(&(network_data[i]));
        switch (packet_class) {
        case SIMPLE:
        {
            SimplePacket* x = (SimplePacket*)malloc(sizeof(SimplePacket));
            memcpy(x, &network_data[i], sizeof(SimplePacket));
            //handleSimplePackets(x);
            sendActionPackets();
            printf("simple");
            i += sizeof(SimplePacket);
            free(x);
            break;
        }
        case STATE:
        {
            StatePacket* y = (StatePacket*)malloc(sizeof(StatePacket));
            memcpy(y, &network_data[i], sizeof(StatePacket));

            // TODO: Replace with actual handler
            if (y->packet_type == MESSAGE) {
                std::cout << "[Player " << iter->first << "]: " << y->payload << std::endl;
            }
            //handleActionPackets(y);
            sendActionPackets();
            i += sizeof(StatePacket);
            free(y);
            break;
        }
        case MOVE:
        {
            MovePacket* pack = (MovePacket*)malloc(sizeof(MovePacket));
            memcpy(pack, &network_data[i], sizeof(MovePacket));
            printf("Server received MovePacket, dir = %d\n", pack->state.dir);

            //TODO!! Implement the ability to know which client sent this move packet, currently assuming client 0.
            handleMovePacket(0, pack);

            //only update gamestate when its changed (player moved, died, etc)
            replicateGameState();

            i += sizeof(MovePacket);
            free(pack);
            break;
        }
        default:
            printf("error in packet types\n");
            break;
        }
    }
}

void ServerGame::sendActionPackets()
{
    // send action packet
    /*
    const unsigned int packet_size = sizeof(SimplePacket);
    char packet_data[packet_size];

    SimplePacket packet;
    packet.packet_type = PING;
    */

    const unsigned int packet_size = sizeof(StatePacket);
    char packet_data[packet_size];
    char message[15] = "Hello, player!";
    StatePacket packet;
    packet.packet_type = MESSAGE;
    memcpy(packet.payload, message, sizeof(message));
    char* packet_bytes = packet_to_bytes(&packet, packet_size);

    network->sendToAll(packet_bytes, packet_size);
    free(packet_bytes);
}

//method to translate the model matrix
void moveLocal(glm::mat4& model, const glm::vec3& v) {
    model = model * glm::translate(glm::mat4(1), v);
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
        moveLocal(state.model, glm::vec3(0, 0, -0.2));
        break;
    }
    }
    // Actually do the update...
    player_states[client_id] = state;
}