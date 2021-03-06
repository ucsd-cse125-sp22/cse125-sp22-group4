#include "Cube.h"

Cube::Cube(glm::vec3 cubeMin, glm::vec3 cubeMax) {
    phongMat = {
        {0.1, 0.1, 0.1, 1},
        {0, 0.9, 0.8, 1},
        {0.1, 0.4, 0.4, 1},
        {0, 0, 0, 1},
        10
    };

    // specify points
    points = {
        // front
        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),

        // back
        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),

        // top
        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),

        // bottom
        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),

        // left
        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),

        // right
        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z)
    };

    // specify normals
    normals = {
        // front
        glm::vec3(0,0,1),
        glm::vec3(0,0,1),
        glm::vec3(0,0,1),
        glm::vec3(0,0,1),

        // back
        glm::vec3(0,0,-1),
        glm::vec3(0,0,-1),
        glm::vec3(0,0,-1),
        glm::vec3(0,0,-1),

        // top
        glm::vec3(0,1,0),
        glm::vec3(0,1,0),
        glm::vec3(0,1,0),
        glm::vec3(0,1,0),

        // bottom
        glm::vec3(0,-1,0),
        glm::vec3(0,-1,0),
        glm::vec3(0,-1,0),
        glm::vec3(0,-1,0),

        // left
        glm::vec3(-1,0,0),
        glm::vec3(-1,0,0),
        glm::vec3(-1,0,0),
        glm::vec3(-1,0,0),

        // right
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0)
    };

    // specify indices
    indices = {
        0,1,2,      0,2,3,      // Front
        4,5,6,      4,6,7,      // Back
        8,9,10,     8,10,11,    // Top
        12,13,14,   12,14,15,   // Bottom
        16,17,18,   16,18,19,   // Left
        20,21,22,   20,22,23,   // Right
    };

    maxX = cubeMax.x > cubeMin.x ? cubeMax.x : cubeMin.x;
    maxZ = cubeMax.z > cubeMin.z ? cubeMax.z : cubeMin.z;
    minX = cubeMin.x < cubeMax.x ? cubeMin.x : cubeMax.x;
    minZ = cubeMin.z < cubeMax.z ? cubeMin.z : cubeMax.z;

    generateBuffers();
    bindBuffers();
}

void Cube::update() {
    spin(5.0f);
}

