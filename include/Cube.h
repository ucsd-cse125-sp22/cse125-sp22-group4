#pragma once
#ifndef CUBE_H
#define CUBE_H

#include "PrimitiveMesh.h"

class Cube : public PrimitiveMesh {
private:

public:
    Cube(glm::vec3 cubeMin=glm::vec3(-1,-1,-1), glm::vec3 cubeMax=glm::vec3(1, 1, 1));
    ~Cube() = default;

    void update() override;
};
#endif // !CUBE_H
