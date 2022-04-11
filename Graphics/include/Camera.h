#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
private:
    const glm::vec3 defaultPos;
    const glm::vec3 defaultLookAt;
    const glm::vec3 defaultUpVec;
    const float defaultFOV;
    const float defaultAspectRatio;
 
public:
    glm::vec3 pos;
    glm::vec3 lookAt;
    glm::vec3 upVec;
    float FOV;
    float aspectRatio;

    glm::mat4 viewProjMat;

    Camera();
    Camera(glm::vec3 _pos, glm::vec3 _lookAt, glm::vec3 _upVec, float _FOV, float _aspectRatio);

    virtual void reset();
    virtual void update();
    virtual void move(glm::vec3 v);
    virtual void zoom(float ratio);
    virtual void yaw(float deg);
    virtual void pitch(float deg);
};
#endif // !CAMERA_HPP
