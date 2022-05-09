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

    playTime = 0; // game play time init

    // I have no idea what the cooldown time is...I put 5 seconds in microseconds
    cooldownTime = 5000000;

    // inaccessible player location for dead mice
    banished = glm::mat4(1);
    moveGlobal(banished, glm::vec3(0, 0, 10));

    // TODO: Should not be hard coded like this.
    player_states[0].modelType = PlayerModelTypes::Dino;
    player_states[1].modelType = PlayerModelTypes::Teapot;
    player_states[2].modelType = PlayerModelTypes::Bunny;
    player_states[3].modelType = PlayerModelTypes::Dino;


    // Load in fake models
    for (int i = 0; i < PLAYER_NUM; ++i) {
        switch (player_states[i].modelType) {
        case PlayerModelTypes::Teapot:
        {
            fakePlayerModels[i] = FakeModel("../../objects/teapot/teapot.obj");
            break;
        }
        case PlayerModelTypes::Dino:
        {
            fakePlayerModels[i] = FakeModel("../../objects/tyra/tyra.obj");
            break;
        }
        case PlayerModelTypes::Bunny:
        {
            fakePlayerModels[i] = FakeModel("../../objects/bunny/bunny.obj");
            break;
        }
        }
    }
}

void ServerGame::assignSpawn(int client_id) {
    PlayerState& state = player_states[client_id];
    switch (client_id) {
    case 0:
        //player 1 starting location
        moveGlobal(state.model, glm::vec3(75, 0, -5));
        oldModels[0] = state.model;
        oldPlayerPositions[0] = state.model;
        break;
    case 1:
        // player 2 starting location
        moveGlobal(state.model, glm::vec3(145, 0, -75));
        spin(state.model, 90);
        oldModels[1] = state.model;
        oldPlayerPositions[1] = state.model;
        break;
    case 2:
        // player 3 starting location
        moveGlobal(state.model, glm::vec3(75, 0, -145));
        spin(state.model, 180);
        oldModels[2] = state.model;
        oldPlayerPositions[2] = state.model;
        break;
    case 3:
        // player 4 starting location
        moveGlobal(state.model, glm::vec3(5, 0, -75));
        spin(state.model, 270);
        oldModels[3] = state.model;
        oldPlayerPositions[3] = state.model;
        break;
    }
    player_states[client_id] = state;
}

void ServerGame::respawnPlayer(int client_id) {
    PlayerState& state = player_states[client_id];
    state.model = oldPlayerPositions[client_id];
    state.alive = true;
    player_states[client_id] = state;
}

void ServerGame::assignSpawnItem() {

    glm::mat4 flagInitLoc = glm::mat4(1);
    time_t t;

    srand((unsigned)time(&t));
    int random = rand() % 5;
    printf("%d spawn\n", random);

    glm::mat4 originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(145, 1, -25));
    oldItemPositions[0] = originalLoc;
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(125, 1, -145));
    oldItemPositions[1] = originalLoc;
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(5, 1, -5));
    oldItemPositions[2] = originalLoc;
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(5, 1, -145));
    oldItemPositions[3] = originalLoc;
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(96, 1, -53));
    oldItemPositions[4] = originalLoc;

    flagInitLoc = oldItemPositions[random];

    flag = new Flag(flagInitLoc, glm::mat4(1));
    flag->randomSpawn = random; // remember new location
}

void ServerGame::respawnItem() {
    int oldNum = flag->randomSpawn;
    int random = flag->randomSpawn;
    time_t t;
    srand((unsigned)time(&t));

    // get new respawn location
    while (oldNum == random) {
        random = rand() % 5;
    }
    printf("%d random\n", random);

    // choose random respawn location
    switch (random) {
    case 0:
        flag->item_state.model = oldItemPositions[0];
        break;
    case 1:
        flag->item_state.model = oldItemPositions[1];
        break;
    case 2:
        flag->item_state.model = oldItemPositions[2];
        break;
    case 3:
        flag->item_state.model = oldItemPositions[3];
        break;
    case 4:
        flag->item_state.model = oldItemPositions[4];
        break;
    }

   
    flag->randomSpawn = random; // remember new location
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
    for (int i = 0; i < PLAYER_NUM; ++i) {
        assignSpawn(i);
        collision_detector->insert(fakePlayerModels[i].getOBB());
        printf("insert %d into cd\n", i);
    }
    flagId = collision_detector->insert(flag->getOBB());
    ServerGame::game_started = true;
}

// i made this function just to get respawn from repeatedly being called
void ServerGame::isTaken() {
    if (ans == 0) {
        ans = 1;
        respawnItem();

    }
}

void ServerGame::collisionStep() {

    //collision_detector.
    for (int i = 0; i < PLAYER_NUM; ++i) {
        collision_detector->update(CollisionDetector::computeOBB(fakePlayerModels[i].getOBB(), player_states[i].model), i);
    }

    collision_detector->update(flag->getOBB(), flagId);

    for (int i = 0; i < PLAYER_NUM; ++i) {
        int hitId = collision_detector->check(i);
        
        // All inserts are in start(), so we know *for now* if it isn't the flag or -1, it's another player
        if (hitId == flagId) {
            //printf("[ServerGame::collisionStep] Player %d hit the flag!\n", i);         
            flag->item_state.hold = i;  
            //mouseDead(i);
        }
        else if (hitId > 0 && i > 0) {
            printf("[ServerGame::collisionStep] Player %d hit player %d!\n", i + 1, hitId + 1);
            player_states[i].model = oldModels[i];
        } else if (i == 0 && hitId > 0) {
            printf("[ServerGame::collisionStep] Player %d killed player %d!\n", i+1, hitId+1);  
            mouseDead(hitId);
                
        } else {
            //printf("[ServerGame::collisionStep] Player %d has no collisions\n", i);
        }
    }
    //printf("\n");
}

void ServerGame::mouseDead(int client_id) {
    PlayerState& state = player_states[client_id];
    
    if (!state.alive)
        return;
    
    if (flag->item_state.hold == client_id) {
        flag->item_state.hold = 5;
        respawnItem();
    }
        
    state.alive = false;
    state.model = banished;
    player_states[client_id] = state;

    start_mouse = timer_mouse.now();
    cooldown.push({ client_id, start_mouse });
    
    //printMat4(player_states[client_id].model);
    // call mouse.die function???
}



void ServerGame::update()
{
  
    // get new clients
    if (network->acceptNewClient(client_id))
    {
        client_id++;
        printf("client %d has been connected to the server\n", client_id);
        if (client_id == 1) {
            // Send game start packets?
            printf("Game start\n");
            start();
        }
    }

    // Receive from clients as fast as possible.
    receiveFromClients();
    //collision should be handled as fast as movement is handled
    if (!ServerGame::game_started) return;
    collisionStep();

    // Calculate tick
    auto stop_time = timer.now();
    auto dt = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    if (dt.count() >= FPS_MAX) {
        replicateGameState();
        start_time = timer.now();
    }

    // game countdown
    auto stop_t = timer_t.now();
    auto test = std::chrono::duration_cast<std::chrono::seconds>(stop_t - start_t);
    playTime = test.count();
    
    checkCooldownOver();
}

void ServerGame::checkCooldownOver() {
    // check cooldown queue for dead mice
    if (!cooldown.empty()) {
        int numDeadMice = cooldown.size();
        for (int i = 0; i < numDeadMice; i++) {
            std::pair<int, std::chrono::steady_clock::time_point> deadMouse = cooldown.front();
            int id = deadMouse.first;
            auto stop_mouse = timer_mouse.now();
            auto diff = std::chrono::duration_cast<std::chrono::microseconds>(stop_mouse - deadMouse.second);
            int newTime = cooldownTime - diff.count(); // time left
            
            if (newTime <= 0) { // cooldown is over, mouse can be reborn
                respawnPlayer(id);
                cooldown.pop();
            }
            else { // cooldown is still going, stick mouse to back of queue
                cooldown.pop();
                cooldown.push({ id, deadMouse.second });
            }

        }
    }
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
    oldModels[client_id] = player_states[client_id].model;
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
    oldModels[client_id] = player_states[client_id].model;

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