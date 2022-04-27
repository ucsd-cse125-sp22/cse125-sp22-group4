#pragma once
#include "Graphics/include/GraphicObject.h"

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
private:
	glm::mat4 goalPos; // potentially its own class?
};

