#pragma once
#include "Constants/include/constants.h"
#include <string>
#include "Graphics/include/GraphicObject.h"
#include <chrono>

class Player
{
public:
	glm::mat4 model = glm::mat4(1);
	bool alive = 1;
	// Convert player states to a sendable packet stream
	virtual PlayerState getPacketStream();
};

class Mouse : Player {
public:
	// Mouse needs a timer for death/respawning
	
	bool holdingItem;
	void pickupItem();
	void die();
	void respawn(glm::mat4 respawnPos);
	// bool checkRespawn() ???
	PlayerState getPacketStream() override;
};

class Cat : Player {
public:
	// Change mouse state?
	void attack(Mouse& m);
	PlayerState getPacketStream() override;
};
 
