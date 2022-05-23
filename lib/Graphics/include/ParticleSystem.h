#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Model.h"

struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleSystem{
private:
    std::vector<Particle> particles;
    unsigned int amount;
    GLint shader;
    unsigned int VAO;
    unsigned int TextureID;

    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, GraphicObject& object, glm::vec3 offset = glm::vec3(0.0f));
public:
    ParticleSystem(GLint _shader, const char* textureFile, unsigned int _amount, glm::mat4 _model);
    ~ParticleSystem() = default;

    glm::mat4 model;

    void update(float dt, GraphicObject& object, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f));
    void draw(const glm::mat4& viewProjMat);

};
#endif // !PARTICLESYSTEM_H
