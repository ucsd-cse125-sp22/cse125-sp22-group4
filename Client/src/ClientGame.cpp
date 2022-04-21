#include "ClientGame.h"
#include <string>
#include <iostream>

ClientGame::ClientGame(void)
{
    network = new ClientNetwork();

    // send init packet
    const unsigned int packet_size = sizeof(SimplePacket);

    SimplePacket packet;
    packet.packet_type = INIT_CONNECTION;

    NetworkServices::sendMessage(network->ConnectSocket, packet_to_bytes(&packet, packet_size), packet_size);
}

void ClientGame::sendActionPackets(MovementState s)
{
    if (!s.held) {
        return;
    }

    const unsigned int packet_size = sizeof(MovePacket);
    MovePacket packet;
    packet.packet_type = KEYSTROKE;
    packet.state = s;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);
}

void ClientGame::sendRotationPackets(RotationState s) {
    const unsigned int packet_size = sizeof(RotatePacket);
    RotatePacket packet;
    packet.packet_type = ACTION;
    packet.state = s;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);

    Client::resetRotUpdate();
}

void ClientGame::handleSimplePacket(SimplePacket s) {
    switch (s.packet_type) {
    case INIT_CONNECTION:
    {
        player_id = (unsigned int) s.data;
        loaded = true; // TODO: Figure out if this is the right place to set loaded=True.
        std::cout << "My player id is " << player_id << std::endl;
        Client::setPlayerfromID(player_id);
        break;
    }
    }
}

void ClientGame::update(MovementState s, RotationState r)
{
    // Don't send action events to server if client is not fully loaded
    if (loaded) {
        sendActionPackets(s);
        sendRotationPackets(r);
    }

    SimplePacket packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0)
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) {
    
        ushort packet_class = get_packet_class(&(network_data[i]));
        switch (packet_class) {
        case SIMPLE:
			{
				SimplePacket* x = (SimplePacket*)malloc(sizeof(SimplePacket));
				memcpy(x, &network_data[i], sizeof(SimplePacket));
                handleSimplePacket(*x);
                sendActionPackets(s);
                sendRotationPackets(r);

                i += sizeof(SimplePacket);
                free(x);
                break;
			}
        case GAME_STATE:
            {
                GameStatePacket* packet = (GameStatePacket*)malloc(sizeof(GameStatePacket));
                memcpy(packet, &network_data[i], sizeof(GameStatePacket));

                updateModels(packet->player_states);
                Client::updateCam();

                //printf("Client received gamestate with coordinates: x = %f, y = %f, z = %f\n", mat[3][0], mat[3][1], mat[3][2]);

                i += sizeof(GameStatePacket);
                free(packet);
                break;
            }
            default:
                printf("error in packet types\n");
                break;
        }
    }
}

void ClientGame::updateModels(PlayerState states[PLAYER_NUM]) {
    for (int i = 0; i < PLAYER_NUM; i++) {
        players[i]->setModel(states[i].model);
    }
}

//for debugging move elsewhere later
void ClientGame::printMat4(glm::mat4 mat) {
    printf("%f, %f, %f, %f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    printf("%f, %f, %f, %f\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    printf("%f, %f, %f, %f\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    printf("%f, %f, %f, %f\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

void ClientGame::setPlayers(Model** p) {
    players = p;
}

unsigned int ClientGame::getPlayer_id() {
    printf("got player id: %d\n", player_id);
    return player_id;
}