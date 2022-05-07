#pragma once
#include "Graphics/include/GraphicObject.h"
#include "Graphics/include/CollisionDetector.h"
#include "Constants/include/constants.h"

class Item
{
public:
	// Stop rendering on map
	void pickUp();

	// Put item back at spawn point
	void respawn(glm::mat4 respawnPos);
	
	// Check if item is in goal position
	virtual bool checkGoal();
	glm::mat4 model = glm::mat4(1); // position of item

private:
	bool beingHeld; // determine if item is in maze or not
};

class Flag : Item {
public:
	// Check if Flag is within goalPos
	bool checkGoal() override;
	OBB getOBB();

	Flag(glm::mat4 initialPos, glm::mat4 endPos);
	~Flag();
	ItemState item_state;
	int randomSpawn;

private:
	glm::mat4 initPos;
	glm::mat4 goalPos; // potentially its own class?
};

