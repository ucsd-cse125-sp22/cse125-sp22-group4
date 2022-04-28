#pragma once
#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include "Camera.h"
#include "GraphicObject.h"

class ThirdPersonCamera : public Camera {
private:
    void updatePos();

public:
    GraphicObject* player;

    ThirdPersonCamera(GraphicObject* _player);
    ~ThirdPersonCamera() = default;

    void reset() override;
    void update() override;
    void translateLeft(float amount);
    void translateRight(float amount);
    void translateForward(float amount);
    void translateBackward(float amount);
};
#endif // !THIRD_PERSON_CAMERA_H