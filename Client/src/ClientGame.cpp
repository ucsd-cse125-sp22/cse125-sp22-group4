#include "ClientGame.h"
#include <string>
#include <iostream>

ClientGame::ClientGame(void)
{
    network = new ClientNetwork();
    start_time = timer.now();
    game_ended = false;

    for (int i = 0; i < PLAYER_NUM; ++i)
        playerSelection[i] = NONE;

    // send init packet
    const unsigned int packet_size = sizeof(SimplePacket);

    SimplePacket packet;
    packet.packet_type = INIT_CONNECTION;

    NetworkServices::sendMessage(network->ConnectSocket, packet_to_bytes(&packet, packet_size), packet_size);
}

bool ClientGame::checkPlayerSelection() {
    if (playerSelectionUpdated) {
        playerSelectionUpdated = false;
        return true;
    }
    return false;
}

std::array<int, PLAYER_NUM> ClientGame::getPlayerSelection() {
    // Because we can't pass around normal arrays w/o pointers ):
    std::array<int, PLAYER_NUM> selectionArr;
    for (int i = 0; i < PLAYER_NUM; ++i) {
        selectionArr[i] = playerSelection[i];
    }
    return selectionArr;
}

void ClientGame::sendActionPackets(MovementState s)
{
    if (!s.held) {
        return;
    }

    const unsigned int packet_size = sizeof(MovePacket);
    MovePacket packet;
    packet.state = s;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);
}

void ClientGame::sendRotationPackets(RotationState s) {
    if (!s.moving) {
        return;
    }

    const unsigned int packet_size = sizeof(RotatePacket);
    RotatePacket packet;
    packet.state = s;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);

    Client::resetRotUpdate();
}

void ClientGame::sendHideStart() {
    const unsigned int packet_size = sizeof(SimplePacket);
    SimplePacket packet;
    packet.packet_type = HIDE_START;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);
}

void ClientGame::sendGameStart() {
    const unsigned int packet_size = sizeof(SimplePacket);
    SimplePacket packet;
    packet.packet_type = GAME_START;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);
}

void ClientGame::sendPlayerSelect(int choice) {
    const unsigned int packet_size = sizeof(SimplePacket);
    SimplePacket packet;
    packet.packet_type = PLAYER_SELECT;
    packet.data = (char)choice; // It's ok >:)

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);
}

void ClientGame::sendPairCount(int pair1, int pair2) {
    const unsigned int packet_size = sizeof(SimplePacket);
    SimplePacket packet;
    packet.packet_type = PAIR_COUNT;
    packet.data = (char)pair1;
    packet.data2 = (char)pair2;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_bytes, packet_size);
    free(packet_bytes);
}

void ClientGame::handleSimplePacket(SimplePacket s) {
    switch (s.packet_type) {
    case INIT_CONNECTION:
    {
        player_id = (unsigned int) s.data;
        loaded = true; // TODO: Figure out if this is the right place to set loaded=True.
        std::cout << "My player id is " << player_id << std::endl;
        Client::setPlayerfromID(player_id);

        // If player is joinning late, they should still learn if game is alive/dead.
        if (s.data2) {
            Client::setGameStart();
        }
        break;
    }
    case GAME_START:
    {
        // Notify player starts.
        // Init player timer.
        printf("Got game start packet\n");
        Client::setGameStart();
        break;
    }
    case GAME_END: 
    {
        printf("THE GAME HAS ENDED\n");
        if (game_ended) {
            printf("The game has already ended\n");
            break;
        }


        // For determining how long to display end screen
        game_ended = true;
        time_ended = timer.now();
        
        if (s.data == CAT_WIN) {
            printf("cat win\n");
            gameEnd(1, CAT_WIN);
        } else {
            printf("mice win\n");
            gameEnd(1, MOUSE_WIN);
        }
        break;
    }
    case HIDE_START:
    {
        Client::hideStartScreen();
    }
    case PLAYER_COUNT:
    {
        setNumPlayers((int) s.data);
    }
    }
}

void ClientGame::update(MovementState s, RotationState r)
{
    // Don't send action events to server if client is not fully loaded
    auto stop_time = timer.now();
    auto dt = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

    if (game_ended) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(stop_time - time_ended).count();
        if (elapsed >= END_SCREEN_TIME) {
            game_ended = false;
            Client::restore();
        }
    }

    if (dt.count() >= FPS_MAX && loaded) {
        start_time = timer.now();
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

                i += sizeof(SimplePacket);
                free(x);
                break;
			}
        case GAME_STATE:
            {
                GameStatePacket* packet = (GameStatePacket*)malloc(sizeof(GameStatePacket));
                memcpy(packet, &network_data[i], sizeof(GameStatePacket));
                updateModels(packet->player_states);

                if (packet->item_state.hold == PLAYER_NUM + 1) {
                    setItem(packet->item_state.model);
                }
                else{
                    glm::mat4 playerModel = players[packet->item_state.hold]->getModel();
                    glm::mat4 newItemModel = playerModel * glm::translate(glm::vec3(0, 1.8f, 0));
                    newItemModel = newItemModel * glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                    newItemModel = newItemModel * glm::scale(glm::vec3(0.2f));
                    setItem(newItemModel);
                }

                Client::setMovingState(packet->player_states);
                Client::setItemHold(packet->item_state.hold, packet->item_state.taskSuccess);
                setFinalDest(packet->game.dest, packet->game.finalDestRotateTime);
                Client::updateItem2Location(packet->item2_state.model);
                Client::updateItem3Location(packet->item3_state.model);
                Client::setStationaryItemCountdown(packet->item2_state.timeLeftHolding, packet->item2_state.holdId, packet->item2_state.taskSuccess);
                Client::setStationaryItem2Countdown(packet->item3_state.timeLeftHolding, packet->item3_state.holdId, packet->item3_state.taskSuccess);
                Client::setCatViewItem(packet->game.catViewItem);
                Client::setAliveStatus(packet->player_states);
                //printMat4(packet->item_state.model);
                
                setTime(packet->game.gameTime);

                i += sizeof(GameStatePacket);

                free(packet);
                break;
            }
        case SELECTION_PACKET:
            {
                printf("selection pcket coming!\n");
                SelectionPacket* packet = (SelectionPacket*)malloc(sizeof(SelectionPacket));
                memcpy(packet, &network_data[i], sizeof(SelectionPacket));
                memcpy(playerSelection, packet->player_choices, sizeof(playerSelection));

                playerSelectionUpdated = true;
                for (int i = 0; i < PLAYER_NUM; ++i) {
                    printf("this is playerSelection[%d]: %d\n", i, playerSelection[i]);
                }

                i += sizeof(SelectionPacket);
                free(packet);
                break;
            }
        default: 
            {
                printf("error in packet types %d\n", packet_class);
                break;
            }
        }
    }
}

void ClientGame::setItem(glm::mat4 location) {
    Client::updateItemLocation(location);
}

void ClientGame::setNumPlayers(int p) {
    Client::setNumPlayers(p);
}

void ClientGame::setTime(int t) {
    Client::updateTime(t);
}

void ClientGame::setFinalDest(glm::mat4 location, int f) {
    Client::setFinalDest(location, f);
}

void ClientGame::gameEnd(int gameOver, int catWon) {
    Client::setGameOver(gameOver, catWon);
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