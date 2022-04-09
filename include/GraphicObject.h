#pragma once
#ifndef GRAPHIC_OBJECT_H
#define GRAPHIC_OBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

class GraphicObject {
public:
    virtual ~GraphicObject() = default;
    virtual void draw(const glm::mat4& viewProjMat, GLuint shader) const = 0;
    virtual void update() = 0;
    virtual void move(glm::vec3 v) = 0;
};

#endif // !GRAPHIC_OBJECT_H
