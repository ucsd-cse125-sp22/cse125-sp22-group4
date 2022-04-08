// may need #include "stdafx.h" in visual studio
//#include "stdafx.h"
#include "ServerGame.h"
// used for multi-threading
#include <process.h>

void serverLoop(void*);
ServerGame* server;


int main()
{

    // initialize the server
    server = new ServerGame();

    // create thread with arbitrary argument for the run function
    serverLoop((void*)12);
}

void serverLoop(void* arg)
{
    while (true)
    {
        server->update();
    }
}
