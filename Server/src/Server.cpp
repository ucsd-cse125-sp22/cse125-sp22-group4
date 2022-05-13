// used for multi-threading
#include <process.h>
#include <iostream>
#include "ServerGame.h"
#include<map>

void serverLoop();
ServerGame* server;


int main()
{
    // initialize the server
    server = new ServerGame();
    serverLoop();
}

void serverLoop()
{
    while (true)
    {
        server->update();
    }
}
