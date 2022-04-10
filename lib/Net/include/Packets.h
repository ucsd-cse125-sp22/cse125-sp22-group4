#pragma once

struct NormalPacket {
	int packet_type = 1;
	char payload[32];
};

struct BigPacket {
	int packet_type = 2;
	char payload[64];
};

int get_packet_type(char* data);
char* packet_to_bytes(void* packet, size_t original_size);
void* bytes_to_packet(char* data);


