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
#include "spdlog/spdlog.h"
#include "CollisionDetector.h"

#define MAX_BONE_INFLUENCE 4

class GraphicObject {
public:
    virtual ~GraphicObject() = default;
    virtual void draw(const glm::mat4& viewProjMat,
                      const glm::mat4& transform,
                      GLuint shader) const = 0;
    virtual void update() = 0;
    virtual void spin(float deg) = 0;
    virtual void scale(const glm::vec3& rate) = 0;
    virtual void moveLocal(const glm::vec3& v) = 0;
    virtual void moveGlobal(const glm::vec3& v) = 0;
    virtual void setModel(const glm::mat4& m) = 0;
    virtual const glm::mat4& getModel() const = 0;
    virtual OBB getOBB() const = 0;
};

#endif // !GRAPHIC_OBJECT_H
