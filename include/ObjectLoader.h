#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"
#include <string>

using namespace glm;

class ObjectLoader : public Mesh {
private:

public:
    ObjectLoader(std::string filename);
    ~ObjectLoader() = default;

    void update() override;
    void spin(float deg);
    void translate(glm::vec3 amount);
    glm::mat4 getmodel() {
        return model;
    }
};
#endif // !CUBE_H