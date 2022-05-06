#include "Player.h"

void Mouse::pickupItem() {
	this->holdingItem = true;
	// Handle item rendering
}

void Mouse::die() {
	this->alive = false;
	this->start_mouse = this->timer_mouse.now();

	// Unrender mouse/put dead mouse
	if (this->holdingItem) {
		this->holdingItem = false;
		// RESPAWN ITEM
	}
}

void Mouse::respawn(glm::mat4 respawnPos) {
	this->alive = true;
	this->model = respawnPos;
	// Move to spawn point, as given by parameter
}


// Later, cat and mouse will have different
// types of player states...?
PlayerState Mouse::getPacketStream() {
	PlayerState ps;
	ps.alive = this->alive;
	ps.model = this->model;
	return ps;
}


void Cat::attack(Mouse& m) {
	m.die(); // ):
}

// Later, cat and mouse will have different
// types of player states...?
PlayerState Cat::getPacketStream() {
	PlayerState ps;
	ps.alive = this->alive;
	ps.model = this->model;
	return ps;
}
