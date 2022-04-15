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

    //printf("sending MovementState from the client, dir = %d\n", s.dir);

    const unsigned int packet_size = sizeof(MovePacket);
    char packet_data[packet_size];
    char message[14] = "Hello, world!";
    MovePacket packet;
    packet.packet_type = KEYSTROKE;
    packet.state = s;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);
}

void ClientGame::update(MovementState s)
{
    sendActionPackets(s);
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
                //handleSimplePackets(x);
                sendActionPackets(s);
                i += sizeof(SimplePacket);

                free(x);
                break;
			}
        case STATE:
			{
				StatePacket* y = (StatePacket*)malloc(sizeof(StatePacket));
				memcpy(y, &network_data[i], sizeof(StatePacket));
                //handleActionPackets(y);
                sendActionPackets(s);
                if (y->packet_type == MESSAGE) {
                    std::cout << "[Server]: " << y->payload << std::endl;
                }

                i += sizeof(StatePacket);
                free(y);
                break;
			}
        case GAME_STATE:
            {
                GameStatePacket* packet = (GameStatePacket*)malloc(sizeof(GameStatePacket));
                memcpy(packet, &network_data[i], sizeof(GameStatePacket));

                glm::mat4 mat = packet->player_states->model;

                //TODO!! Implement client numbers, so that client knows which player state is their own.
                //       Currently assuming client 0.
                printf("Client received gamestate with coordinates: x = %f, y = %f, z = %f\n", mat[3][0], mat[3][1], mat[3][2]);

                player->setModel(mat);

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

//for debugging move elsewhere later
void ClientGame::printMat4(glm::mat4 mat) {
    printf("%f, %f, %f, %f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    printf("%f, %f, %f, %f\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    printf("%f, %f, %f, %f\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    printf("%f, %f, %f, %f\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}

void ClientGame::setPlayer(Model* p) {
    player = p;
}
