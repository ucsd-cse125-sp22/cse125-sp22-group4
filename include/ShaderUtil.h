#ifndef SHADER_UTIL_H

#include "glm/glm.hpp"

struct PhongMaterial {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 emission;
    float shininess;
};

#endif // !SHADER_UTIL_H
