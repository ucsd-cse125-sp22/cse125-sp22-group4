#ifndef PACKETS 
#define PACKETS

#include "Constants/include/constants.h"
#pragma once
#define MAX_PACKET_SIZE 1000000

typedef unsigned short ushort;

enum PacketClasses {
	SIMPLE,
	STATE,
	MOVE,
};

enum PacketTypes {
	/* Simple Packet Types */
	INIT_CONNECTION,
	PING,
	KEYSTROKE,
	/* Keystroke Packet Types */

	/* State Packet Types*/
	ACTION,
	MESSAGE,
};


// For first connections, pinging, etc.
// Anything that doesn't need a payload
struct SimplePacket {
	ushort packet_class = SIMPLE;
	ushort packet_type;
	char data;
};

struct MovePacket {
	ushort packet_class = MOVE;
	ushort packet_type;
	MovementState state;
};



// Note: We add player_id field here because we expect this packet to be replicated
// to other clients. Prior packet structs did not have player_id because server already
// knows packet origin.
struct StatePacket {
	ushort packet_class = STATE;
	ushort packet_type;
	ushort player_id;
	char payload[32];
};


ushort get_packet_class(char* data);
char* packet_to_bytes(void* packet, size_t original_size);
//int get_packet_type(char* data);
//void* bytes_to_packet(char* data);
#endif

