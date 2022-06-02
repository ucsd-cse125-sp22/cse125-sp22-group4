//#include "stdafx.h" 
#include "ServerGame.h"
#include <iostream>
#include "yaml-cpp/yaml.h"

unsigned int ServerGame::client_id;
bool ServerGame::game_started;


void moveGlobal(glm::mat4& model, const glm::vec3& v) {
    model = glm::translate(glm::mat4(1), v) * model;
}

void spin(glm::mat4& model, float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void flip(glm::mat4& model, float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(1.0f, 0.0f, 0.0f));
}

void scale(glm::mat4& model, const glm::vec3& rate) {
    model = model * glm::scale(rate);
}


ServerGame::ServerGame() :
    catSpeed(DEFAULT_CATSPEED), mouseSpeed(DEFAULT_MOUSESPEED),
    roundLengthSec(DEFAULT_ROUNDLENGTHSEC),
    cooldownTimeSec(DEFAULT_COOLDOWNTIMESEC),
    player0DevMode(DEFAULT_PLAYER0DEVMODE), catViewItemSec(DEFAULT_CATVIEWITEMSEC),
    pointsToWin(DEFAULT_POINTSTOWIN)
{
    gameAlive = false; // Will be toggled on/off depending on the round
    game_started = false; // Will only be false once.
    this->ticksSinceConfigCheck = 0;

    for (int i = 0; i < PLAYER_NUM; ++i)
        playerSelection[i] = NONE;

    // Update settings from config file!
    updateFromConfigFile();

    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen
    network = new ServerNetwork();
    start_time = timer.now();
    maze = new Maze();
    collision_detector = new CollisionDetector();

    playTime = 0; // game play time init
    points = 0;

    // inaccessible player location for dead mice
    banished = glm::mat4(1);
    moveGlobal(banished, glm::vec3(75, 20, -75));

    // TODO: Should not be hard coded like this.
    player_states[0].modelType = PlayerModelTypes::Cat;
    player_states[1].modelType = PlayerModelTypes::Mice;
    player_states[2].modelType = PlayerModelTypes::Mice;
    player_states[3].modelType = PlayerModelTypes::Mice;


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
        case PlayerModelTypes::Cat:
        {
            fakePlayerModels[i] = FakeModel("../../objects/cat/cat.obj");
            break;
        }
        case PlayerModelTypes::Mice:
        {
            fakePlayerModels[i] = FakeModel("../../objects/mouse/mouse.obj");
            break;
        }
        }
    }

    //load maze collision
    scene = new SceneLoader("../../objects/new_maze_collision/scene.txt");
    sceneObjects = scene->fakeLoad("../../objects/new_maze_collision/");

    spdlog::info("finished loading sceneObjects, count: {}", sceneObjects.size());
}

void ServerGame::assignSpawn(int client_id) {
    PlayerState& state = player_states[client_id];
    state.model = glm::mat4(1);
    switch (client_id) {
    case 0:
        //player 1 starting location house
        moveGlobal(state.model, glm::vec3(188.3f, 0, 5.83f));
        spin(state.model, 90);
        oldModels[0] = state.model;
        oldPlayerPositions[0] = state.model;
        break;
    case 1:
        // player 2 starting location well
        moveGlobal(state.model, glm::vec3(-187.0f, 0, -3.3f));
        spin(state.model, 270);
        oldModels[1] = state.model;
        oldPlayerPositions[1] = state.model;
        break;
    case 2:
        // player 3 starting location stone
        moveGlobal(state.model, glm::vec3(23.16f, 0, 186.4f));
        spin(state.model, 180);
        oldModels[2] = state.model;
        oldPlayerPositions[2] = state.model;
        break;
    case 3:
        // player 4 starting location flower
        moveGlobal(state.model, glm::vec3(-37.88f, 0, -175.0f));
        spin(state.model, 270);
        oldModels[3] = state.model;
        oldPlayerPositions[3] = state.model;
        break;
    }
    player_states[client_id] = state;
}

void ServerGame::respawnPlayer(int client_id) {
    PlayerState& state = player_states[client_id];
    glm::mat4 newModel = glm::mat4(1);
    if (client_id == 0)
        return;
    //state.model = oldPlayerPositions[client_id];
    time_t t;

    srand((unsigned)time(&t));
    int random = rand() % 3;

    switch (client_id) {
    case 1:
        if (random == 0) {
            moveGlobal(newModel, glm::vec3(145, 0, -75));
            spin(newModel, 90);
        }
        else if (random == 1) {
            moveGlobal(newModel, glm::vec3(35, -3, -105));
            spin(newModel, 90);
        }
        else {
            moveGlobal(newModel, glm::vec3(115, -3, -45));
            spin(newModel, 90);
        }
        break;

    case 2:
        if (random == 0) {
            moveGlobal(newModel, glm::vec3(75, -3, -145));
            spin(newModel, 180);
            
        }
        else if (random == 1) {
            moveGlobal(newModel, glm::vec3(115, -3, -85));
            spin(newModel, 180);
        }
        else {
            moveGlobal(newModel, glm::vec3(55, -3, -15));
            spin(newModel, 180);
        }
        break;
        
    case 3:
        if (random == 0) {
            moveGlobal(newModel, glm::vec3(5, -3, -75));
            spin(state.model, 270);
        }
        else if (random == 1) {
            moveGlobal(newModel, glm::vec3(85, -3, -105));
            spin(state.model, 270);
        }
        else {
            moveGlobal(newModel, glm::vec3(145, -3, -105));
            spin(state.model, 270);
        }
        break;
    }

    state.model = newModel;
    state.alive = true;
    player_states[client_id] = state;
}

void ServerGame::assignSpawnItem() {

    glm::mat4 flagInitLoc = glm::mat4(1);
    time_t t;

    srand((unsigned)time(&t));
    int random = rand() % 5;
    printf("%d item spawn\n", random);
    
    // location 1
    glm::mat4 originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(-127.5, 1, -202.5));
    spin(originalLoc, 90);
    originalLoc = originalLoc * glm::scale(glm::vec3(0.2f));
    oldItemPositions[0] = originalLoc;

    // location 2
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(-142.5, 1, -127.5));
    spin(originalLoc, 180);
    originalLoc = originalLoc * glm::scale(glm::vec3(0.2f));
    oldItemPositions[1] = originalLoc;

    // location 3
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(157.5, 1, -97.5));
    spin(originalLoc, 90);
    originalLoc = originalLoc * glm::scale(glm::vec3(0.2f));
    oldItemPositions[2] = originalLoc;

    // location 4
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(-67.5, 1, -67.5));
    spin(originalLoc, 90);
    originalLoc = originalLoc * glm::scale(glm::vec3(0.2f));
    oldItemPositions[3] = originalLoc;

    //location 5
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(67.5, 1, 217.5));
    originalLoc = originalLoc * glm::scale(glm::vec3(0.2f));
    oldItemPositions[4] = originalLoc;
    //random = 0;
    flagInitLoc = oldItemPositions[random];


    flag = new Flag(flagInitLoc, glm::mat4(1));
    //flag->item_state.model = flag->item_state.model * glm::scale(glm::vec3(0.2f));
    flag->randomSpawn = random; // remember new location
}

void ServerGame::assignSpawnItem2() {

    glm::mat4 initLoc = glm::mat4(1);
    time_t t;

    srand((unsigned)time(&t));
    int random = rand() % 5;
    printf("%d spawn\n", random);

    // location 1
    glm::mat4 originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(-67.5, 2, 157.5));
    scale(originalLoc, glm::vec3(5));
    //spin(originalLoc, 90);
    oldItem2Positions[0] = originalLoc;
    // location 2
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(-187.5, 2, 142.5));
    scale(originalLoc, glm::vec3(5));
    //spin(originalLoc, 180);
    oldItem2Positions[1] = originalLoc;
    // location 3
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(172.5, 2, -217.5));
    spin(originalLoc, 90);
    scale(originalLoc, glm::vec3(5));
    oldItem2Positions[2] = originalLoc;
    // location 4
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(67.5, 2, 67.5));
    spin(originalLoc, 180);
    scale(originalLoc, glm::vec3(5));
    oldItem2Positions[3] = originalLoc;
    //location 5
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(157.5, 2, 112.5));
    scale(originalLoc, glm::vec3(5));
    oldItem2Positions[4] = originalLoc;

    initLoc = oldItem2Positions[random];
   
    stationary->setPosition(initLoc);
    stationary->randomSpawn = random;
}

void ServerGame::assignSpawnItem3() {

    glm::mat4 initLoc = glm::mat4(1);
    time_t t;

    srand((unsigned)time(&t));
    int random = rand() % 5;
    printf("%d spawn\n", random);

    // location 1
    glm::mat4 originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(-142.5, 1, 127.5));
    scale(originalLoc, glm::vec3(5));
    spin(originalLoc, 90);
    oldItem3Positions[0] = originalLoc;
    // location 2
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(-112.5, 1, 7.5));
    scale(originalLoc, glm::vec3(5));
    spin(originalLoc, 90);
    oldItem3Positions[1] = originalLoc;
    // location 3
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(37.5, 1, -202.5));
    spin(originalLoc, 90);
    scale(originalLoc, glm::vec3(5));
    oldItem3Positions[2] = originalLoc;
    // location 4
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(187.5, 1, -67.5));
    spin(originalLoc, 90);
    scale(originalLoc, glm::vec3(5));
    oldItem3Positions[3] = originalLoc;
    //location 5
    originalLoc = glm::mat4(1);
    moveGlobal(originalLoc, glm::vec3(127.5, 1, 157.5));
    spin(originalLoc, 90);
    scale(originalLoc, glm::vec3(5));
    oldItem3Positions[4] = originalLoc;

    initLoc = oldItem3Positions[random];
    //initLoc = oldItem3Positions[0];

    stationary2->setPosition(initLoc);
    stationary2->randomSpawn = random;
}

void ServerGame::respawnItem2() {
    int oldNum = stationary->randomSpawn;
    int random = stationary->randomSpawn;
    time_t t;
    srand((unsigned)time(&t));

    if (stationary->model[3][1] > 10)
        return;

    // get new respawn location
    while (oldNum == random) {
        random = rand() % 5;
    }
    printf("%d random item2\n", random);

    // choose random respawn location
    stationary->setPosition(oldItem2Positions[random]);
    stationary->randomSpawn = random; // remember new location
}

void ServerGame::respawnItem3() {
    int oldNum = stationary2->randomSpawn;
    int random = stationary2->randomSpawn;
    time_t t;
    srand((unsigned)time(&t));

    if (stationary2->model[3][1] > 10)
        return;

    // get new respawn location
    while (oldNum == random) {
        random = rand() % 5;
    }
    printf("%d random item3\n", random);

    // choose random respawn location
    stationary2->setPosition(oldItem3Positions[random]);
    stationary2->randomSpawn = random; // remember new location
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
    printf("%d random item1\n", random);

    // choose random respawn location
    flag->item_state.model = oldItemPositions[random];
    flag->randomSpawn = random; // remember new location
}

void ServerGame::setupStationaryObjectives() {
    stationary = new SitAndHoldObjective(10.0);
    assignSpawnItem2();

    stationary2 = new SitAndHoldObjective(10.0);
    assignSpawnItem3();
}

void ServerGame::start() {
    gameAlive = true;
    playTime = 0; // Reset play time
    points = 0; // Reset points

    // Reset player selections for next run
    for (int i = 0; i < PLAYER_NUM; ++i)
        playerSelection[i] = NONE;

    // Announce game start
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
    spawnFinalDestination();
    setupStationaryObjectives();

    // Move players to spawn and setup collision

    if (!ServerGame::game_started) {
        for (int i = 0; i < PLAYER_NUM; ++i) {
            collision_detector->insert(fakePlayerModels[i].getOBB());
            printf("insert %d into cd\n", i);
        }
        flagId = collision_detector->insert(flag->getOBB());
        stationaryId = collision_detector->insert(stationary->getOBB());
        stationary2Id = collision_detector->insert(stationary2->getOBB());


        for (auto& wall : sceneObjects) {
            wallOBBs.push_back(collision_detector->insert(wall->getOBB()));
        }

        ServerGame::game_started = true;
    }

    // Move to original positions
    //
    for (int i = 0; i < PLAYER_NUM; ++i) {
        assignSpawn(i);
        player_states[client_id].alive = true;
    }
}

void ServerGame::spawnFinalDestination() {
    time_t t;
    srand((unsigned)time(&t));
    int random = rand() % 4;
    printf("spawn final dest %d\n", random);
    oldFinalDestinations[0] = glm::translate(glm::mat4(1), glm::vec3(220, 0, -220)); // fallenstar
    oldFinalDestinations[1] = glm::translate(glm::mat4(1), glm::vec3(-220, 0, -220)); // raccoon
    oldFinalDestinations[2] = glm::translate(glm::mat4(1), glm::vec3(220, 0, 220)); // bearl
    oldFinalDestinations[3] = glm::translate(glm::mat4(1), glm::vec3(-220, 0, 220)); // sungod

    destModel = oldFinalDestinations[random];
    finalDestLoc = random;
}

bool ServerGame::isAtFinalDest(int hitId) {

    switch (finalDestLoc) {
    case 0: // fallen star
        if (hitId == 307)
            return true;
        break;
    case 1:
        if (hitId == 311) // racoon
            return true;
        break;
    case 2: // bearl
        if (hitId == 340 || hitId == 341)
            return true;
        break;
    case 3: // sungod
        if (hitId == 309 || hitId == 328)
            return true;
        break;

    }
    return false;
}

void ServerGame::respawnFinalDest() {
    // this is broken!!!! must be fixed if going to be used
    time_t t;
    srand((unsigned)time(&t));
    int random = finalDestLoc;
    
    while (random == finalDestLoc)
        random = rand() % 4;
    printf("respawn final dest %d\n", random);
    OBB collisionModel;

    // TODO: adjust OBB accordingly
    switch (random) {
    case 0: // fallenstar
        //destModel = oldFinalDestinations[0];
        break;
    case 1: // geisel
        //destModel = oldFinalDestinations[1];
        break;
    case 2: // bearl
        //destModel = oldFinalDestinations[2];
        break;
    case 3: // sungod
        //destModel = oldFinalDestinations[3];
        break;
    }
     
    finalDestLoc = random;
    OBB bearOBB = FakeModel("../../objects/bunny/bunny.obj").getOBB();
    collision_detector->update(CollisionDetector::computeOBB(bearOBB, destModel), goalId);
    //bearId = collision_detector->insert(CollisionDetector::computeOBB(bearOBB, destModel));
}


void ServerGame::collisionStep() {

    //collision_detector.
    for (int i = 0; i < PLAYER_NUM; ++i) {
        collision_detector->update(CollisionDetector::computeOBB(fakePlayerModels[i].getOBB(), player_states[i].model), i);
    }

    // Update objective bounding boxes
    collision_detector->update(stationary->getOBB(), stationaryId);
    collision_detector->update(stationary2->getOBB(), stationary2Id);
    collision_detector->update(flag->getOBB(), flagId);

    for (int i = 0; i < PLAYER_NUM; ++i) {
        bool in_stationary = false;
        bool in_stationary2 = false;
        for (int hitId : collision_detector->check(i)) {
            if (hitId == flagId) { // item 1
                if (i == CAT_ID && !player0DevMode) break; // Cat can't hold item!
                flag->item_state.hold = i;
                if (!flag_taken) {
                    flag_taken = true;
                    start_finalDest = timer_finalDest.now();
                }
            }
            else if (hitId == stationaryId) { // item 2
                if (i == CAT_ID && !player0DevMode) break; // Cat can't hold objective!
                stationary->interact(i, true);
                in_stationary = true;
            }
            else if (hitId == stationary2Id) { // item 3
                if (i == CAT_ID && !player0DevMode) break; // Cat can't hold objective!
                stationary2->interact(i, true);
                in_stationary2 = true;
            }
            else if (isAtFinalDest(hitId)) { // item 1 is carried here
                printf("[ServerGame::collisionStep] Player %d hit bear!\n", i + 1);
                player_states[i].model = oldModels[i];
                //printf("flag %d i %d\n", flag->item_state.hold, i);
                if (flag->item_state.hold == i) {
                    // The player with flag hit goal!
                    printf("Item brought to goal!\n");
                    ++points;
                    // TODO: This respawns the item, we need to take it out of the map!
					flag->item_state.hold = 5;
                    flag->item_state.taskSuccess = true;
                    moveGlobal(flag->item_state.model, glm::vec3(0, 100.0f, 0)); // just making it go away..
                    flag_taken = false;
                    finalDestTime = -1;
                }
            }
            else if (hitId > 0 && i > 0 && hitId < PLAYER_NUM) {
                printf("[ServerGame::collisionStep] Player %d hit player %d!\n", i + 1, hitId + 1);
                player_states[i].model = oldModels[i];
            }
            else if (i == 0 && hitId > 0 && hitId < PLAYER_NUM) {
                printf("[ServerGame::collisionStep] Player %d killed player %d!\n", i + 1, hitId + 1);
                mouseDead(hitId);
            }

            
            for (int wallnum : wallOBBs) {
                if (hitId == wallnum) {
                    player_states[i].model = oldModels[i];
                    spdlog::info("player hit wall number: {}", wallnum);
                    break;
                }
            }
            
        }

        // Tell stationary that player is not in
        if (!in_stationary) {
            stationary->interact(i, false);
        }

        if (!in_stationary2) {
            stationary2->interact(i, false);
        }
    }
        
}

void ServerGame::mouseDead(int client_id) {
    PlayerState& state = player_states[client_id];
    
    if (!state.alive)
        return;      
    
    if (flag->item_state.hold == client_id) {
        flag->item_state.hold = 5;
        respawnItem();
        flag_taken = false;
        finalDestTime = -1;
    }
      
    state.alive = false;
    state.model = banished;
    player_states[client_id] = state;
    catViewItem = true;

    lastMouseDeath = timer_mouse.now();
    cooldown.push({ client_id, timer_mouse.now() });
}

void ServerGame::checkStationaryObjectives() {
    if (pair1 == 3) {
        moveGlobal(stationary->model, glm::vec3(0, 100, 0));
        printf("Stationary: Completed!\n");
        stationary->checkAward();
        pair1 = -1;
        ++points;
    }
    if (pair2 == 3) {
        moveGlobal(stationary2->model, glm::vec3(0, 100, 0));
        printf("Stationary2: Completed!\n");
        stationary->checkAward();
        pair2 = -1;
        ++points;
    }
 /*   if (stationary->checkAward()) {
        moveGlobal(stationary->model, glm::vec3(0, 100, 0));
        printf("Stationary: Completed!\n");
        ++points;
        
    }
    if (stationary2->checkAward()) {
        moveGlobal(stationary2->model, glm::vec3(0, 100, 0));
        printf("Stationary2: Completed!\n");
        ++points;
    }*/
}

void ServerGame::update()
{
    // get new clients
    if (network->acceptNewClient(client_id))
    {
        client_id++;
        printf("client %d has been connected to the server\n", client_id);
    }

    // Receive from clients as fast as possible.
    receiveFromClients();
    //collision should be handled as fast as movement is handled
    if (!ServerGame::game_started) {
        // Still need to tell clients game state...
        auto stop_time = timer.now();
        auto dt = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
        if (dt.count() >= FPS_MAX) {
            start_time = timer.now();
            updatePlayerCount();
        }
        return;
    }

    // Calculate tick
    auto stop_time = timer.now();
    auto dt = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    if (dt.count() >= FPS_MAX) {
        start_time = timer.now();

        // game countdown
        auto stop_t = timer_t.now();
        auto test = std::chrono::duration_cast<std::chrono::seconds>(stop_t - start_t);
        playTime = test.count();

        // check to rotate stationary items
        //if (playTime == 60 && !firstTimer) {
        //    firstTimer = true;
        //    respawnItem();
        //    // make sure player isn't at stationary
        //    if (stationary->getProgress() == 0.0) 
        //        respawnItem2();
        //    if (stationary2->getProgress() == 0.0)
        //        respawnItem3();
        //}
        //else if (playTime == 120 && !secondTimer) {
        //    secondTimer = true;
        //    respawnItem();
        //    if (stationary->getProgress() == 0.0)
        //        respawnItem2();
        //    if (stationary2->getProgress() == 0.0)
        //        respawnItem3();
        //}

        // TODO: round length is fixed as 180 on client.
        if (this->roundLengthSec - playTime <= 0 && gameAlive) {
            announceGameEnd(CAT_WIN);
            gameAlive = false;
        }
       
        checkStationaryObjectives();
        checkCooldownOver();
        //checkFinalDestRotates();
        replicateGameState();

        for (int i = 0; i < PLAYER_NUM; i++) {
            player_states[i].moving = false;
        }

        if (points == pointsToWin && gameAlive) {
            announceGameEnd(MOUSE_WIN);
            gameAlive = false;
        }

        // Check config file every sec
        ++this->ticksSinceConfigCheck;
        if (this->ticksSinceConfigCheck % 60) {
            updateFromConfigFile();
            this->ticksSinceConfigCheck = 0;
        }
    }
}

void ServerGame::checkFinalDestRotates() {
    if (flag_taken) {
        auto stop_finalDest = timer_finalDest.now();
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(stop_finalDest - start_finalDest);
        finalDestTime = finalDestRotatesTime - diff.count();
        if (finalDestTime < 0) {
            respawnFinalDest();
            start_finalDest = timer_finalDest.now();
        }
    }
}

void ServerGame::updateFromConfigFile() {
    YAML::Node config;
    try {
       config = YAML::LoadFile(CONFIG_FILE);
    }
    catch (YAML::BadFile e) {
        return;
    }

    if (config["server"]) {
        auto serverConf = config["server"];
        if (serverConf["catSpeed"]) this->catSpeed = serverConf["catSpeed"].as<double>();
        if (serverConf["mouseSpeed"]) this->mouseSpeed = serverConf["mouseSpeed"].as<double>();
        // TODO: if we want accurate Client timers, this should be extracted to be a shared config var
        if (serverConf["roundLengthSec"]) this->roundLengthSec = serverConf["roundLengthSec"].as<double>();
        if (serverConf["cooldownTimeSec"]) this->cooldownTimeSec = serverConf["cooldownTimeSec"].as<double>();
        if (serverConf["catViewItemSec"]) this->catViewItemSec = serverConf["catViewItemSec"].as<double>();
		if (serverConf["player0DevMode"]) this->player0DevMode = serverConf["player0DevMode"].as<bool>();
	}
}

void ServerGame::checkCooldownOver() {

    // check cooldown queue for dead mice
    if (!cooldown.empty()) {
        int numDeadMice = cooldown.size();
        for (int i = 0; i < numDeadMice; i++) {
            std::pair<int, std::chrono::steady_clock::time_point> deadMouse = cooldown.front();
            int id = deadMouse.first;
            auto stop_mouse = timer_mouse.now();
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(stop_mouse - deadMouse.second);
            int newTime = this->cooldownTimeSec - diff.count(); // time left in cooldown
            
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

    if (catViewItem) {
        // catViewItem is true if the cat has viewed the item for less time than catViewItemSec
        catViewItem = std::chrono::duration_cast<std::chrono::seconds>(timer_mouse.now() - lastMouseDeath).count() < catViewItemSec;
    }
}

void ServerGame::updatePlayerCount() {
    const unsigned int packet_size = sizeof(GameStatePacket);
    GameStatePacket packet;
    memcpy(packet.player_states, player_states, sizeof(player_states));
    packet.game.numPlayers = client_id;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);
    network->sendToAll(packet_bytes, packet_size);
    free(packet_bytes);
}

//broadcast game state to all clients
void ServerGame::replicateGameState() {
    const unsigned int packet_size = sizeof(GameStatePacket);
    GameStatePacket packet;
    memcpy(packet.player_states, player_states, sizeof(player_states));
   
    packet.item_state = flag->item_state;


    // ==== STATIONARY TASKS ====
    packet.item2_state.model = stationary->model;
    packet.item2_state.timeLeftHolding = stationary->getProgress();
    packet.item2_state.taskSuccess = stationary->taskSuccess;
    for (int i = 0; i < 4; i++)
        packet.item2_state.holdId[i] = stationary->holdId[i];

    packet.item3_state.model = stationary2->model;
    packet.item3_state.timeLeftHolding = stationary2->getProgress();
    packet.item3_state.taskSuccess = stationary2->taskSuccess;
    for (int i = 0; i < 4; i++)
        packet.item3_state.holdId[i] = stationary2->holdId[i];
    // ==========================

    packet.game.gameTime = playTime;
    packet.game.numPlayers = client_id;
    packet.game.dest = destModel;
    packet.game.finalDestRotateTime = finalDestTime;
    packet.game.catViewItem = catViewItem || player0DevMode;

    char* packet_bytes = packet_to_bytes(&packet, packet_size);

    network->sendToAll(packet_bytes, packet_size);
    free(packet_bytes);
}

// broadcast game end
void ServerGame::announceGameEnd(bool winner) {
    const unsigned int packet_size = sizeof(SimplePacket);
    SimplePacket packet;
    packet.packet_type = GAME_END;
    packet.data = (char)winner;

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
        while (i < data_length) {

            //check collision as fast as packets are received
                if (ServerGame::game_started) {
                    collisionStep();
                }

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
                break;
            }
            case ROTATE:
            {
                RotatePacket* pack = (RotatePacket*)malloc(sizeof(RotatePacket));
                memcpy(pack, &network_data[i], sizeof(RotatePacket));
                handleRotatePacket(client_id, pack);

                //if (ServerGame::game_started) {
                //    collisionStep();
               //}

                i += sizeof(RotatePacket);
                free(pack);
                break;
            }
            default:
                printf("error in packet types\n");
                break;
            }
        }
    }
}

//method to translate the model matrix
// TODO: Make use of graphics library instead. Have an object wrap the player's positions
// and use methods to manipulate.
void ServerGame::moveLocal(glm::mat4& model, const glm::vec3& v) {
    model = glm::translate(model, v);
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
            
            // Send back ID to client
            id_packet.data = (char)iter->first;
            // Tell client if game is up
            id_packet.data2 = (char)gameAlive;
            char* packet_bytes = packet_to_bytes(&id_packet, sizeof(id_packet));
            network->sendToSocket(player_socket, packet_bytes, sizeof(id_packet));
		}
        break;
    }
    case GAME_START:
    {
        // If we recieve a GAME_START packet from client, bounce to other clients!
        if (gameAlive) {
            break;
        }

        printf("Game start!\n");
        start();

        // Bounce packet to other clients
        char* packet_bytes = packet_to_bytes(packet, sizeof(SimplePacket));
        size_t packet_size = sizeof(SimplePacket);
        network->sendToAll(packet_bytes, packet_size);
        break;
    }
    case PLAYER_SELECT:
    {
        if (gameAlive) {
            break;
        }

        int index = (int)packet->data;

        // Deselect packet
        if (index == NONE) {
            // Unselect for player
            for (int i = 0; i < PLAYER_NUM; ++i) {
                if (playerSelection[i] == client_id) {
                    playerSelection[i] = NONE;
                    break;
                }
            }
        }
        else {
            // Select for player
            if (playerSelection[index] == NONE)
                playerSelection[index] = client_id;
        }

        bool allSelected = true;
        for (int id : playerSelection) {
            if (id == NONE) {
                allSelected = false;
                break;
            }
        }

        if (allSelected) {
            printf("All players have selected, game can start!\n");
            start();

            // Bounce packet to other clients
            char* packet_bytes = packet_to_bytes(packet, sizeof(SimplePacket));
            size_t packet_size = sizeof(SimplePacket);
            network->sendToAll(packet_bytes, packet_size);
            break;
        }


        // Not everyone has selected, just replicate selection changes.
        size_t packet_size = sizeof(SelectionPacket);
        SelectionPacket* selectPacket = (SelectionPacket*) malloc(packet_size);
        selectPacket->packet_class = SELECTION_PACKET;
        memcpy(selectPacket->player_choices, playerSelection, sizeof(playerSelection));

        char* packet_bytes = packet_to_bytes(selectPacket, packet_size);
        network->sendToAll(packet_bytes, packet_size);
        break;
    }
    case HIDE_START:
    {
        // Bounce packet to other clients
        char* packet_bytes = packet_to_bytes(packet, sizeof(SimplePacket));
        size_t packet_size = sizeof(SimplePacket);
        network->sendToAll(packet_bytes, packet_size);
        break;
    }
    case PAIR_COUNT:
    {
        printf("receiving pair count packet!\n");

        pair1 = (int)packet->data;
        pair2 = (int)packet->data2;
        break;
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
    bool obstacle = false;

    glm::mat4 rotationMatrix = glm::rotate(packet->state.delta, glm::vec3(0.0f, 1.0f, 0.0f));
   // bool obstacle = maze->rotateBlock(state.modelType, client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2], (rotationMatrix[2][0] / rotationMatrix[2][2]));
  
    if (!obstacle) {
        state.model = state.model * rotationMatrix;
        player_states[client_id] = state;
    }
    
}

//Update player_state from move packet.
void ServerGame::handleMovePacket(int client_id, MovePacket* packet) {
    PlayerState state = player_states[client_id];
    oldModels[client_id] = player_states[client_id].model;
    double playerSpeed = client_id == CAT_ID ? catSpeed : mouseSpeed;

    if (!state.alive || !packet->state.held) {
        return;
    }
    
    glm::vec3 netDirection = glm::vec3(0);
    for (int i = 0; i < 4; ++i) {
        bool obstacle = false;
        int direction = packet->state.dir[i];
        if (!direction)
            continue;

        /*
        switch (i) {
            case LEFT:
            {
                obstacle = maze->leftBlock(state.modelType, client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);
                break;
            }
            case RIGHT:
            {
                obstacle = maze->rightBlock(state.modelType, client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);
                break;
            }
            case BACK:
            {
                obstacle = maze->backwardsBlock(state.modelType, client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);
                break;
            }
            case FORWARD:
            {
                obstacle = maze->forwardBlock(state.modelType, client_id, state.model[3][0], state.model[3][2], state.model[2][0], state.model[2][2]);
                break;
            }
        }
        */

        if (!obstacle) {
            netDirection += DIR_TO_VEC[i];
        }
        else {
            state.model = oldModels[client_id];
            player_states[client_id] = state;
        }
    }

    if (glm::length2(netDirection) < 0.1) {
        state.moving = false;
        return;
    }

    state.moving = true;

    netDirection = glm::normalize(netDirection);
    glm::vec3 delta = netDirection * (float) playerSpeed;
    moveLocal(state.model, delta);

    // Actually do the update...
    player_states[client_id] = state;
}

void ServerGame::printMat4(glm::mat4 mat) {
    printf("%f, %f, %f, %f\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    printf("%f, %f, %f, %f\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    printf("%f, %f, %f, %f\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    printf("%f, %f, %f, %f\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}