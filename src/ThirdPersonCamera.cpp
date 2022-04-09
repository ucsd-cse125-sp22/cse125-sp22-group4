#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(ObjectLoader* _player) {
	player = _player;
	vec3 playerPos = player->getmodel()[3];
	vec3 camPos = playerPos;
	camPos[2] += 8;
	camPos[1] += 3;
	pos = camPos;
	lookAt = glm::vec3(0, 0, 0);
	upVec = vec3(0, 1, 0);
	FOV = 60.0f;
	aspectRatio = 16.0f / 9.0f;
}

void ThirdPersonCamera::update() {

	viewProjMat = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(pos, lookAt, glm::normalize(upVec));
}

/**
 * Control camera yaw
 *
 * @param   angle   Angle to yaw
**/
void ThirdPersonCamera::yaw(float angle) {
	// yaw against (0, 1, 0) if upVec pointing upward, yaw against (0, -1, 0) otherwise
	glm::vec3 axis = glm::vec3(0, 1, 0);
	axis = upVec.y > 0 ? axis : -axis;

	glm::mat4 rotMatrix = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	pos = glm::vec3(rotMatrix * glm::vec4(pos - lookAt, 1)) + lookAt;
	upVec = glm::vec3(rotMatrix * glm::vec4(upVec, 0));
}

/**
 * Control camera pitch
 *
 * @param   angle   Angle to pitch
**/
void ThirdPersonCamera::pitch(float angle) {
	// pitch against camera X coordinate
	glm::vec3 cameraZ = normalize(pos - lookAt);
	glm::vec3 axis = normalize(cross(upVec, cameraZ));

	glm::mat4 rotMatrix = glm::rotate(mat4(1), angle, axis);
	pos = glm::vec3(rotMatrix * glm::vec4(pos - lookAt, 1)) + lookAt;
	upVec = glm::vec3(rotMatrix * glm::vec4(upVec, 0));
}

void ThirdPersonCamera::translateForward(float amount) {
	//move camera towards lookat on x and z.
	glm::vec3 worldMove = amount * normalize(pos - vec3(lookAt[0], pos[1], lookAt[2]));
	pos += worldMove;
	lookAt += worldMove;
}

void ThirdPersonCamera::translateBackward(float amount) {
	//move camera backwards to lookat on x and z.
	glm::vec3 worldMove = -amount * normalize(pos - vec3(lookAt[0], pos[1], lookAt[2]));
	pos += worldMove;
	lookAt += worldMove;
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

//TODO
void ThirdPersonCamera::reset() {
	vec3 playerPos = player->getmodel()[3];
	vec3 camPos = playerPos;
	camPos[2] += 8;
	camPos[1] += 3;
	pos = camPos;
	lookAt = playerPos;
	upVec = vec3(0, 1, 0);
	FOV = 60.0f;
	aspectRatio = 16.0f / 9.0f;
}