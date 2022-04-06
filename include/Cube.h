#pragma once
#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"

class Cube : public Mesh {
private:

public:
    Cube(glm::vec3 cubeMin=glm::vec3(-1,-1,-1), glm::vec3 cubeMax=glm::vec3(1, 1, 1));
    ~Cube() = default;

    void update() override;
    void spin(float deg);
};
#endif // !CUBE_H
