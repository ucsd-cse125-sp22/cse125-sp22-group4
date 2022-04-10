#include <string>
#include <iostream>
#include "Packets.h"

ushort get_packet_class(char* data) {
    ushort result;
    memcpy(&result, data, sizeof(result));
    return result;
}

char* packet_to_bytes(void* packet, size_t original_size) {
    char* out = (char*)malloc(original_size);
    memcpy(out, packet, original_size);
    return out;
}

/*
int get_packet_type(char* data) {
    int result;
    memcpy(&result, data, sizeof(result));
    return result;
}
*/

/*
void* bytes_to_packet(char* data) {
    switch (get_packet_type(data)) {
    case INIT_CONNECTION: case PING: case KEYSTROKE:
    {
        SimplePacket* x = (SimplePacket*)malloc(sizeof(SimplePacket));
        memcpy(x, data, sizeof(SimplePacket));
        return (void*)x;
    }
    case ACTION: case MESSAGE:
    {
        StatePacket* y = (StatePacket*)malloc(sizeof(StatePacket));
        memcpy(y, data, sizeof(StatePacket));
        return (void*)y;
    }
    }
}
*/
