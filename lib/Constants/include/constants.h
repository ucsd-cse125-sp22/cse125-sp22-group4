#pragma once
#include "Graphics/include/GraphicObject.h"
#define PLAYER_NUM 4
#define ITEM_NUM_LOC 5
#define MAX_PACKET_SIZE 1000000
#define MOUSE_THRESH 0.00001

enum Directions { FORWARD, BACK, LEFT, RIGHT };

struct MovementState {
	int dir;
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

struct GameState {
	int gameTime;
};
