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

    void reset() override;
    void translateLeft(float amount);
    void translateRight(float amount);
    void translateForward(float amount);
    void translateBackward(float amount);
};
#endif // !CUBE_H