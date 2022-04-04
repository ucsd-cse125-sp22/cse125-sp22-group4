#pragma once
#ifndef CUBE_H
#define CUBE_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include <vector>

class Cube {
private:
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    glm::vec3 color;
    glm::mat4 model;


    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    void generateBuffers();
    void bindBuffers();

public:
    Cube(glm::vec3 cubeMin=glm::vec3(-1,-1,-1), glm::vec3 cubeMax=glm::vec3(1, 1, 1));
    ~Cube();

    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update();
    void spin(float deg);
    void setColor(glm::vec3 c);
};
#endif // !CUBE_H
