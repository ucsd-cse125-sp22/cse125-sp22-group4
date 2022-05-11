// used for multi-threading
#include <process.h>
#include <iostream>
#include "ServerGame.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <filesystem>
#include<map>

void serverLoop();
ServerGame* server;


int main()
{
    // This should be in repo root
    double playerSpeed;
    YAML::Node config = YAML::LoadFile("../../config.yaml");
    if (config["server"]) {
        auto serverConf = config["server"];
        playerSpeed = serverConf["speed"].as<double>();
    }
    else {
        spdlog::info("Cannot find config file");
    }
    // initialize the server
    server = new ServerGame(playerSpeed);
    serverLoop();
}

void serverLoop()
{
    while (true)
    {
        server->update();
    }
}
