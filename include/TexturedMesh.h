#ifndef TEXTURED_MESH_H
#define TEXTURED_MESH_H

#include "GraphicObject.h"

#include <string>
#include <vector>
#include <iostream>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class TexturedMesh : public GraphicObject {
private:
    GLuint VAO, VBO, EBO;
    glm::mat4 model;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    void generateBuffers();
    void bindBuffers();

public:
    TexturedMesh();
    TexturedMesh(const TexturedMesh& old);
    TexturedMesh(const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        const std::vector<Texture>& textures);
    ~TexturedMesh();
    virtual void draw(const glm::mat4& viewProjMat, GLuint shader) const override;
    virtual void update() override;
};
#endif
