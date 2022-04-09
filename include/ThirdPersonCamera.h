#pragma once
#ifndef THIRDPERSONCAMERA_H
#define THIRDPERSONCAMERA_H

#include "Camera.h"
#include "ObjectLoader.h"

class ThirdPersonCamera : public Camera {
private:
    ObjectLoader* player;

public:
    ThirdPersonCamera(ObjectLoader* _player);
    ~ThirdPersonCamera() = default;

    void update();
    void yaw(float angle);
    void pitch(float angle);
    void reset();
    void translateForward(float amount);
    void translateBackward(float amount);
    void translateLeft(float amount);
    void translateRight(float amount);
};
#endif // !CUBE_H