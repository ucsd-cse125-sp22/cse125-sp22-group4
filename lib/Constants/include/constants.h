#pragma once
#include "Graphics/include/GraphicObject.h"
#define PLAYER_NUM 4
#define ID_LEN 9
#define CHECK_COOKIES true

enum Directions { FORWARD, BACK, LEFT, RIGHT };

struct MovementState {
	int dir;
	bool held;
};

struct RotationState {
	glm::mat4 rotationalMatrix;
};

struct PlayerState {
	glm::mat4 model = glm::mat4(1);
	bool alive = 1;
};