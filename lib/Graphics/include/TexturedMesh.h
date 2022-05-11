#ifndef TEXTURED_MESH_H
#define TEXTURED_MESH_H

#include "ShaderUtil.h"
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

    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct BoneInfo
{
    int id;
    glm::mat4 offset;
};

class TexturedMesh : public GraphicObject {
private:
    GLuint VAO, VBO, EBO;
    glm::mat4 model;
    PhongMaterial phongMat;
    float maxX, maxZ, minX, minZ;

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
                 const std::vector<Texture>& textures,
                 const PhongMaterial& _phongMat);
    ~TexturedMesh();

    virtual void draw(const glm::mat4& viewProjMat,
                      const glm::mat4& transform,
                      GLuint shader) const override;
    virtual void update() override;
    virtual void spin(float deg) override;
    virtual void scale(const glm::vec3& rate) override;
    virtual void moveLocal(const glm::vec3& v) override;
    virtual void moveGlobal(const glm::vec3& v) override;
    virtual void setModel(const glm::mat4& m) override;
    virtual const glm::mat4& getModel() const override;
    virtual OBB getOBB() const override;
};
#endif
