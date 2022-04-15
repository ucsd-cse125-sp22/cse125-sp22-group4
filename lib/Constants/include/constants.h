#pragma once
#include "Graphics/include/GraphicObject.h"
#define PLAYER_NUM 4

enum Directions { FORWARD, BACK, LEFT, RIGHT };

struct MovementState {
	int dir;
	bool held;
};

struct PlayerState {
	glm::mat4 model = glm::mat4(1);
	bool alive = 1;
};