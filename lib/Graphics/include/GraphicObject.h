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
    virtual void spin(float deg) = 0;
    virtual void scale(glm::vec3 rate) = 0;
    virtual void moveLocal(glm::vec3 v) = 0;
    virtual void moveGlobal(glm::vec3 v) = 0;
    virtual const glm::mat4& getModel() const = 0;
};

#endif // !GRAPHIC_OBJECT_H
