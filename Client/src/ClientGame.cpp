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

    printf("sending MovementState from the client, dir = %d\n", s.dir);
    // send action packet
    /*
    const unsigned int packet_size = sizeof(SimplePacket);
    char packet_data[packet_size];

    SimplePacket packet;
    packet.packet_type = PING;

    NetworkServices::sendMessage(network->ConnectSocket, packet_to_bytes(&packet, packet_size), packet_size);
    */
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