#include "Camera.h"

/**
 * Default constructor with
 * pos(0, 0, 10), lookAt(0, 0, 0), upVec(0, 1, 0), FOV(45) and aspectRatio(16/9) and 
**/
Camera::Camera() :
    Camera(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 
    45.0f, 16.0f/9.0f) { }

/**
 * Camera constructor taking in pos, lookAt, upVec and aspectRatio
 *
 * @param   _pos            Position of the camera
 * @param   _lookAt         Point where camera is pointing to
 * @param   _upVec          Upward vector of the camera
 * @param   _FOV            FOV angle of the camera
 * @param   _aspectRatio    Aspect ratio of the camera
**/
Camera::Camera(glm::vec3 _pos, glm::vec3 _lookAt, glm::vec3 _upVec, float _FOV, float _aspectRatio) :
    pos(_pos), lookAt(_lookAt), upVec(_upVec),
    FOV(_FOV), aspectRatio(_aspectRatio),
    defaultPos(_pos), defaultLookAt(_lookAt), defaultUpVec(_upVec),
    defaultFOV(_FOV), defaultAspectRatio(_aspectRatio) {
    // compute view projection matrix
    view = glm::lookAt(pos, lookAt, glm::normalize(upVec));
    projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 1000.0f);
    viewProjMat = projection * view;
}

/**
 * Reset camera parameters to default states
**/
void Camera::reset() {
    pos = defaultPos;
    lookAt = defaultLookAt;
    upVec = defaultUpVec;
    FOV = defaultFOV;
    aspectRatio = defaultAspectRatio;
}

/**
 * Update view projection matrix based on current parameter values
**/
void Camera::update() {
    projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 1000.0f);
    view = glm::lookAt(pos, lookAt, glm::normalize(upVec));
    viewProjMat = projection * view;
}

/**
 * Move the camera
 *
 * @param   v   Vector of displacement
**/
void Camera::move(glm::vec3 v) {
    // transform this vector to world coordinate
    glm::vec3 cameraZ = normalize(pos - lookAt);
    glm::vec3 cameraX = normalize(cross(upVec, cameraZ));
    glm::vec3 cameraY = cross(cameraZ, cameraX);
    glm::vec3 worldMove = v.x * cameraX + v.y * cameraY + v.z * cameraZ;
    pos += worldMove;
    lookAt += worldMove;
}

/**
 * Zoom by the ratio
 *
 * @param   ratio   Ratio to zoom
**/
void Camera::zoom(float ratio) {
    glm::vec3 lookAtToPos = pos - lookAt;
    pos = lookAt + ratio * lookAtToPos;
}

/**
 * Control camera yaw
 *
 * @param   angle   Angle to yaw
**/
void Camera::yaw(float deg) {
    // yaw against (0, 1, 0) if upVec pointing upward, yaw against (0, -1, 0) otherwise
    glm::vec3 axis = glm::vec3(0, 1, 0);
    axis = upVec.y > 0 ? axis : -axis;

    glm::mat4 rotMatrix = glm::rotate(glm::mat4(1), glm::radians(deg), axis);
    pos = glm::vec3(rotMatrix * glm::vec4(pos - lookAt, 1)) + lookAt;
    upVec = glm::vec3(rotMatrix * glm::vec4(upVec, 0));
}

/**
 * Control camera pitch
 *
 * @param   angle   Angle to pitch
**/
void Camera::pitch(float deg) {
    // pitch against camera X coordinate
    glm::vec3 cameraZ = normalize(pos - lookAt);
    glm::vec3 axis = normalize(cross(upVec, cameraZ));

    glm::mat4 rotMatrix = glm::rotate(glm::mat4(1), glm::radians(deg), axis);
    pos = glm::vec3(rotMatrix * glm::vec4(pos - lookAt, 1)) + lookAt;
    upVec = glm::vec3(rotMatrix * glm::vec4(upVec, 0));
}
