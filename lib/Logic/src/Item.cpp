#include "Item.h"

void Item::pickUp()
{
	this->beingHeld = true;
	// Stop rendering on map
}

void Item::respawn(glm::mat4 respawnPos)
{
	this->beingHeld = false;
	// put back at spawn point, as given by parameter
}

bool Item::checkGoal()
{
	return false;
}

bool Flag::checkGoal()
{
	// TODO: set goalPos to be a range/area that item needs to be in
	return (this->model == this->goalPos);
}
