#pragma once
#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {
    SIMPLE = 3,
    INIT_CONNECTION = 1,
    ACTION_EVENT = 2,
};

class SimplePacket {
    public:
        const int packet_type = SIMPLE;

        // Return char buffer of "this" as bytes
        char* serialize() {
            char* buf = (char*) malloc(sizeof(this));
            memcpy(buf, this, sizeof(this));
            return buf;
        }

        // Convert char buffer back to original object 
        void deserialize(char* data) {
            memcpy(this, data, sizeof(data));
        }
};


class MovementPacket : public SimplePacket {
public:
    const int packet_type = ACTION_EVENT;
    char payload[32];
    MovementPacket() {}

    // Load character buffer into self
    MovementPacket(char* buf) {
        memcpy(this, buf, sizeof(buf));
    }
};

struct GenericPacket {
    unsigned short packet_type;
};

struct Packet {

    unsigned short packet_type;
    char* body;

    // Convert packet to bytes, stored in data
    void serialize(char* data) {
        this->body = (char*) malloc(sizeof(data));
        memcpy(data, this, sizeof(this));
    }

    // Convert 
    void deserialize(char* data) {
        memcpy(this, data, sizeof(data));
    }
};