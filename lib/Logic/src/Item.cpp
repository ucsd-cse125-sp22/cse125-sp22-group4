#include "Item.h"

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

Flag::Flag(glm::mat4 initialPos, glm::mat4 endPos) 
{
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
	return CollisionDetector::computeOBB({ {1,1},{-1,1}, {-1,-1}, {1,-1} }, model);
}