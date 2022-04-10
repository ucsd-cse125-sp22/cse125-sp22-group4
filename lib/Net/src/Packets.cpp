#include "pch.h"
#include <string>
#include <Packets.h>

int get_packet_type(char* data) {
    int result;
    memcpy(&result, data, sizeof(result));
    return result;
}

char* to_bytes(void* packet, size_t original_size) {
    char* out = (char*)malloc(original_size);
    memcpy(out, packet, original_size);
    return out;
}

void* to_packet(char* data) {
    switch (get_packet_type(data)) {
    case 1:
    {
        NormalPacket* x = (NormalPacket*)malloc(sizeof(NormalPacket));
        memcpy(x, data, sizeof(NormalPacket));
        return (void*)x;
    }
    case 2:
    {
        BigPacket* y = (BigPacket*)malloc(sizeof(BigPacket));
        memcpy(y, data, sizeof(BigPacket));
        return (void*)y;
    }
    }
}
