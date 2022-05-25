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

struct ParticleProperty {
    unsigned int amount              = 500;   //total amount of particles
    float        Life                = 1.0f; //how long it can live
    glm::vec3    Velocity            = glm::vec3(0, -5, 0);     //initial velocity of particle
    glm::vec3    useRandomVelocity   = glm::vec3(0, 0, 0);      //the amount of randomness each frame on velocity
    int          randomPositionRange = 5;    //random respawn position from -range to range
    bool         randomColor         = 0;    //use random color each particle or not
    float        colorFade           = 0.5f; //how quick the particle will fade, bigger the quicker
    unsigned int blendMethod         = 0;    //how to blend particles, 0 for covering up, 1 for additive
};

class ParticleSystem{
private:
    std::vector<Particle> particles;
    GLint shader;
    unsigned int VAO;
    unsigned int TextureID;

    unsigned int amount;
    float particleLife;
    glm::vec3 particleVelocity;
    glm::vec3 useRandomVelocity;
    int randomPositionRange;
    bool randomColor;
    float colorFade;
    unsigned int blendMethod;

    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, glm::vec3 offset = glm::vec3(0.0f));
public:
    ParticleSystem(GLint _shader, const char* textureFile, ParticleProperty properties);
    ~ParticleSystem() = default;

    glm::mat4 model;

    void update(float dt, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f));
    void draw(const glm::mat4& viewProjMat, const glm::vec3& Camera_Right, const glm::vec3& Camera_Up);

};
#endif // !PARTICLESYSTEM_H
