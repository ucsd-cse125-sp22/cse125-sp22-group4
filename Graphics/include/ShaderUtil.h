#ifndef SHADER_UTIL_H

#include "glm/glm.hpp"

struct PhongMaterial {
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 emission;
    float shininess;
};

namespace color {
    static const PhongMaterial GREEN{
        {0.1, 0.1, 0.1, 1},
        {0.1, 0.8, 0.5, 1},
        {0.1, 0.8, 0.4, 1},
        {0, 0, 0, 1},
        100
    };
}

#endif // !SHADER_UTIL_H
