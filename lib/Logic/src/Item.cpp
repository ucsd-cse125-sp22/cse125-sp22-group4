#include "Item.h"

// ====== ITEM ======
void Item::pickUp()
{
	this->beingHeld = true;
	// Stop rendering on map
}

void Item::respawn(glm::mat4 respawnPos)
{
	this->beingHeld = false;
	this->model = respawnPos;
	// put back at spawn point, as given by parameter
}

bool Item::checkGoal()
{
	return false;
}

// ====== FLAG ======
Flag::Flag(glm::mat4 initialPos, glm::mat4 endPos) 
{
	model = initialPos;
	initPos = initialPos;
	goalPos = endPos;
	item_state = {
		initialPos,
		5
	};
}

bool Flag::checkGoal()
{
	// TODO: set goalPos to be a range/area that item needs to be in
	return (this->model == this->goalPos);
}

OBB Flag::getOBB() {
	// TODO: Fix hard coded OBB.
	return CollisionDetector::computeOBB({ {1,1},{-1,1}, {-1,-1}, {1,-1} }, item_state.model);
}

// ====== SitAndHoldObjective ======
OBB SitAndHoldObjective::getOBB() {
	// TODO: Fix hard coded OBB.
	return CollisionDetector::computeOBB({ {2,2},{-2,2}, {-2,-2}, {2,-2} }, model);
}

void SitAndHoldObjective::interact(int client_id, bool on) {
	if (disabled)
		return;

	// Allow 
	if (on && players_in_zone.find(client_id) == players_in_zone.end()) {
		players_in_zone.insert(client_id);
	}
	else if (!on && players_in_zone.find(client_id) != players_in_zone.end()) {
		players_in_zone.erase(client_id);
	}

	bool pastToggle = toggled;
	toggled = !players_in_zone.empty();
	if (pastToggle == false && toggled == true) {
		start_time = timer.now();
	}
}

float SitAndHoldObjective::getProgress() {
	if (disabled || !toggled)
		return 0.0;

	auto timeNow = timer.now();
	return std::chrono::duration_cast<std::chrono::seconds>(timeNow - start_time).count();
}

bool SitAndHoldObjective::checkAward() {
	// If objective is disabled or not being worked on, disable
	if (disabled || !toggled)
		return false;

	if (getProgress() >= seconds) {
		// Reward! Then disable yourself
		disabled = true;
		return true;
	}
	return false;
}