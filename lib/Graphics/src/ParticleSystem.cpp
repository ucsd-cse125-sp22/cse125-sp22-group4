#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GLint _shader, const char* textureFile, unsigned int _amount, glm::mat4 _model) {
	shader = _shader;
	amount = _amount;
    model = _model;

    //load texture from textureFile
    glGenTextures(1, &TextureID);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureFile, &width, &height, &nrChannels, 0);

    if (!data) {
        spdlog::error("Particle Texture failed to load at: {}", textureFile);
    }

    glBindTexture(GL_TEXTURE_2D, TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    printf("texture id: %u\n", TextureID);
    printf("texture width: %d\n", width);
    printf("texture height: %d\n", height);
    printf("texture nrChannels: %d\n", nrChannels);

    unsigned int VBO;
    static const GLfloat g_vertex_buffer_data[] = {
         -0.5f, -0.5f, 0.0f,
          0.5f, -0.5f, 0.0f,
         -0.5f,  0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindVertexArray(0);


    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

void ParticleSystem::update(float dt, GraphicObject& object, unsigned int newParticles, glm::vec3 offset) {
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], object, offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 10.0f;
        }
    }
}

void ParticleSystem::draw(const glm::mat4& viewProjMat, const glm::vec3& Camera_Right, const glm::vec3& Camera_Up) {
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glUseProgram(shader);
    for (Particle particle : particles)
    {
        if (particle.Life > 0.0f)
        {
            glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, glm::value_ptr(viewProjMat));
            glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(shader, "offset"), 1, glm::value_ptr(particle.Position));
            glUniform4fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(particle.Color));
            glUniform3fv(glGetUniformLocation(shader, "CameraRight_worldspace"), 1, glm::value_ptr(Camera_Right));
            glUniform3fv(glGetUniformLocation(shader, "CameraUp_worldspace"), 1, glm::value_ptr(Camera_Up));
            glActiveTexture(GL_TEXTURE0); 
            glBindTexture(GL_TEXTURE_2D, TextureID);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//get next dead particle
unsigned int lastUsedParticle = 0;
unsigned int ParticleSystem::firstUnusedParticle() {
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    //if it repeatedly hits this case, more particles should be reserved
    lastUsedParticle = 0;
    return 0;
}

void ParticleSystem::respawnParticle(Particle& particle, GraphicObject& object, glm::vec3 offset) {
    glm::vec3 objpos = object.getModel()[3];
    float random_x = ((rand() % 100) - 50) / 10.0f;
    float random_y = ((rand() % 100) - 50) / 10.0f;
    float random_z = ((rand() % 100) - 50) / 10.0f;

    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.Position = glm::vec3(random_x + offset.x, random_y + offset.y, random_z + offset.z);
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 0.5f;
    particle.Velocity = glm::vec3(0, -10, 0);
}
