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
	GAME_STATE,
	ROTATE,
};

enum PacketTypes {
	/* Simple Packet Types */
	INIT_CONNECTION,
	PING,
	KEYSTROKE,
	GAME_START,
	GAME_END,
	/* Keystroke Packet Types */

	/* State Packet Types*/
	ACTION,
	MESSAGE,
};

// Binary enum
enum WIN_STATE {
	MOUSE_WIN,
	CAT_WIN,
};

enum PlayerModelTypes {
	Dino,
	Teapot,
	Bunny,
};


// For first connections, pinging, etc.
// Anything that doesn't need a payload
struct SimplePacket {
	ushort packet_class = SIMPLE;
	ushort packet_type;
	char data;
	char data2;
};

struct MovePacket {
	ushort packet_class = MOVE;
	ushort packet_type;
	MovementState state;
};

struct RotatePacket {
	ushort packet_class = ROTATE;
	ushort packet_type;
	RotationState state;
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

struct GameStatePacket {
	ushort packet_class = GAME_STATE;
	PlayerState player_states[PLAYER_NUM];
	ItemState item_state;
	GameState game;
};

ushort get_packet_class(char* data);
char* packet_to_bytes(void* packet, size_t original_size);
//int get_packet_type(char* data);
//void* bytes_to_packet(char* data);
#endif

