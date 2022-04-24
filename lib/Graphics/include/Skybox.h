#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include "GraphicObject.h"
#include <vector>
#include <string>
#include <stb_image.h>

class Skybox{
private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int id;

public:
    Skybox();
    ~Skybox();

    void draw(const glm::mat4& viewProj, GLuint shader);
    unsigned int load(std::vector<std::string> faces);
};
#endif // !SKYBOX_H
