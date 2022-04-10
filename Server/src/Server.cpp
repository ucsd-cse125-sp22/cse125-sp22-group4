// may need #include "stdafx.h" in visual studio
//#include "stdafx.h"
#include "ServerGame.h"
// used for multi-threading
#include <process.h>
//#include <Network/include/NetworkData.h>
//#include <Network/include/Packets.h>
#include <iostream>

void serverLoop(void*);
ServerGame* server;


int main()
{

    // initialize the server
    server = new ServerGame();
    serverLoop((void*)12);

}

void serverLoop(void* arg)
{
    while (true)
    {
        server->update();
    }
}
