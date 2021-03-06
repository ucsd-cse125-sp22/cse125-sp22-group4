#pragma once
#include "Graphics/include/GraphicObject.h"
#include "Graphics/include/CollisionDetector.h"
#include "Constants/include/constants.h"

#include <math.h>
#include <chrono>
#include <unordered_set>

#define PROGRESS_SCALE 1.15

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

class StationaryObjective {
public:
	virtual OBB getOBB() = 0;
	virtual float getProgress() = 0;
	virtual bool complete() = 0;
	virtual void interact(int client_id, bool on) = 0;
	void setPosition(glm::mat4 pos) {
		model = pos;
	}
	// position of objective
	glm::mat4 model = glm::mat4(1);
	int randomSpawn;
	bool holdId[4] = { false, false, false , false };
	bool taskSuccess = false;

protected:
	StationaryObjective(bool toggled, bool disabled) : toggled(toggled), disabled(disabled) {};

	
	// If the objective is being used by player
	bool toggled;
	// If the objective can still be used
	bool disabled;
};

class SitAndHoldObjective : public StationaryObjective {
public:
	SitAndHoldObjective(float s) : StationaryObjective(false, false), seconds(s) {};
	OBB getOBB() override;
	float getProgress() override;
	bool complete() override;
	void interact(int client_id, bool on) override;
	std::unordered_set<int> players_in_zone;

private:
	float seconds;
	float progress;
	

	std::chrono::steady_clock timer;
	std::chrono::steady_clock::time_point start_time;
};

