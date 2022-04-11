#ifndef PRIMITIVE_MESH_H
#define PRIMITIVE_MESH_H

#include "ShaderUtil.h"
#include "GraphicObject.h"
#include <vector>

class PrimitiveMesh : public GraphicObject {
protected:
    GLuint VAO;
    GLuint VBO_vertices, VBO_normals, EBO;
    glm::mat4 model;
    PhongMaterial phongMat;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    void generateBuffers();
    void bindBuffers();

public:
    PrimitiveMesh();
    PrimitiveMesh(const std::vector<glm::vec3>& _vertices,
        const std::vector<glm::vec3>& _normals,
        const std::vector<unsigned int> _indices,
        const PhongMaterial& _phongMat);
    ~PrimitiveMesh(); 
    void PrimitiveMesh::draw(const glm::mat4& viewProjMat, GLuint shader) const override;

    void spin(float deg);
    void moveLocal(glm::vec3 v);
    void moveGlobal(glm::vec3 v);
    const glm::mat4& getModel() const;
};
#endif // !MESH_H
