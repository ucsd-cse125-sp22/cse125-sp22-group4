#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(GLint _shader, const char* textureFile, ParticleProperty properties) {
	shader = _shader;
	amount = properties.amount;
    particleLife = properties.Life;
    particleVelocity = properties.Velocity;
    useRandomVelocity = properties.useRandomVelocity;
    randomPositionRange = properties.randomPositionRange;
    randomColor = properties.randomColor;
    colorFade = properties.colorFade;
    blendMethod = properties.blendMethod;

    model = glm::mat4(1); 

    //load texture from textureFile
    glGenTextures(1, &TextureID);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
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

    /*
    printf("texture id: %u\n", TextureID);
    printf("texture width: %d\n", width);
    printf("texture height: %d\n", height);
    printf("texture nrChannels: %d\n", nrChannels);
    */

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

void ParticleSystem::update(float dt, unsigned int newParticles, glm::vec3 offset) {
    // add new particles 
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], offset);
    }
    // update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position -= p.Velocity * dt;

            p.Color.a -= dt * colorFade;
            float random_x = -useRandomVelocity.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2* useRandomVelocity.x)));
            float random_y = -useRandomVelocity.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * useRandomVelocity.y)));
            float random_z = -useRandomVelocity.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2 * useRandomVelocity.z)));
            p.Velocity.x += random_x/10.0f;
            p.Velocity.y += random_y/10.0f;
            p.Velocity.z += random_z/10.0f;
        }
    }
}

void ParticleSystem::draw(const glm::mat4& viewProjMat, const glm::vec3& Camera_Right, const glm::vec3& Camera_Up) {
    // use additive blending to give it a 'glow' effect
    glDepthMask(false);
    glEnable(GL_BLEND);
    if (blendMethod == 0) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    
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
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
    }
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDepthMask(true);
   glDisable(GL_BLEND);
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

void ParticleSystem::respawnParticle(Particle& particle, glm::vec3 offset) {

    particle.Life = particleLife;
    particle.Velocity = particleVelocity;

    float random_x = -randomPositionRange.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2*randomPositionRange.x)));
    float random_y = -randomPositionRange.y + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2*randomPositionRange.y)));
    float random_z = -randomPositionRange.z + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2*randomPositionRange.z)));
    particle.Position = glm::vec3(random_x + offset.x, random_y + offset.y, random_z + offset.z);

    float rColor = 0.5f + ((rand() % 100) / 100.0f);
    if (randomColor) {
        particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    }
    else {
        particle.Color = glm::vec4(1, 1, 1, 1.0f);
    }
}
