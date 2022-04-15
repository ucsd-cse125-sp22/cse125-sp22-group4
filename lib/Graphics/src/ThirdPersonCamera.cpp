#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(GraphicObject* _player) {
	player = _player;
	auto& playerModel = player->getModel();
	glm::vec4 playerPos = playerModel[3];
	pos = glm::vec3(playerModel * glm::translate(glm::vec3(0, 3, 8)) * glm::inverse(playerModel) * playerPos);
	lookAt = glm::vec3(playerPos);
	upVec = glm::vec3(0, 1, 0);
	FOV = 60.0f;
	aspectRatio = 16.0f / 9.0f;
}

void ThirdPersonCamera::reset() {
    auto& playerModel = player->getModel();
	glm::vec4 playerPos = playerModel[3];
	pos = glm::vec3(playerModel * glm::translate(glm::vec3(0, 3, 8)) * glm::inverse(playerModel) * playerPos);
	lookAt = glm::vec3(playerPos);
	upVec = glm::vec3(0, 1, 0);
	FOV = 60.0f;
	aspectRatio = 16.0f / 9.0f;
}

void ThirdPersonCamera::translateLeft(float amount) {
	glm::vec3 cameraZ = normalize(pos - lookAt);
	glm::vec3 cameraX = normalize(cross(upVec, cameraZ));
	glm::vec3 worldMove = amount * cameraX;
	pos += worldMove;
	lookAt += worldMove;
}

void ThirdPersonCamera::translateRight(float amount) {
	glm::vec3 cameraZ = normalize(pos - lookAt);
	glm::vec3 cameraX = normalize(cross(upVec, cameraZ));
	glm::vec3 worldMove = -amount * cameraX;
	pos += worldMove;
	lookAt += worldMove;
}

void ThirdPersonCamera::translateForward(float amount) {
	//move camera towards lookat on x and z.
	glm::vec3 worldMove = amount * glm::normalize(pos - glm::vec3(lookAt[0], pos[1], lookAt[2]));
	pos += worldMove;
	lookAt += worldMove;
}

void ThirdPersonCamera::translateBackward(float amount) {
	//move camera backwards to lookat on x and z.
	glm::vec3 worldMove = -amount * glm::normalize(pos - glm::vec3(lookAt[0], pos[1], lookAt[2]));
	pos += worldMove;
	lookAt += worldMove;
}
