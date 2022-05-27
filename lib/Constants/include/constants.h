#pragma once
#include "Graphics/include/GraphicObject.h"
#include <unordered_set>
#define PLAYER_NUM 4
#define ITEM_NUM_LOC 5
#define MAX_PACKET_SIZE 1000000
#define MOUSE_THRESH 0.00001

enum Directions { FORWARD, BACK, LEFT, RIGHT };

struct MovementState {
	bool dir[4];
	bool held;
};

struct RotationState {
	float delta;
	bool moving;
};

struct PlayerState {
	glm::mat4 model = glm::mat4(1);
	bool alive = 1;
	char modelType;
};

struct ItemState {
	glm::mat4 model = glm::mat4(1);
	char hold = 5;
};

struct Item2State {
	glm::mat4 model = glm::mat4(1);
	float timeLeftHolding = 0.0f;
	bool holdId[4];
	bool taskSuccess = false;
};

struct Item3State {
	glm::mat4 model = glm::mat4(1);
	float timeLeftHolding = 0.0f;
	bool holdId[4];
	bool taskSuccess = false;
};

struct GameState {
	int gameTime;
	int numPlayers;
	glm::mat4 dest = glm::mat4(1);
	bool catViewItem;
	int finalDestRotateTime;
};
