#include "PrimitiveMesh.h"

PrimitiveMesh::PrimitiveMesh() {
    VAO = VBO_vertices = VBO_normals = EBO = 0;
    model = glm::mat4(1);
    phongMat = { glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(0), 0 };
}

PrimitiveMesh::PrimitiveMesh(const std::vector<glm::vec3>& _vertices,
                             const std::vector<glm::vec3>& _normals,
                             const std::vector<unsigned int> _indices,
                             const PhongMaterial& _phongMat) {
    vertices = _vertices;
    normals = _normals;
    indices = _indices;
    phongMat = _phongMat;
    generateBuffers();
    bindBuffers();
}

PrimitiveMesh::~PrimitiveMesh() {
    // delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_vertices);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void PrimitiveMesh::draw(const glm::mat4& viewProjMat, GLuint shader) const {
    // activate the shader program
    glUseProgram(shader);
    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, glm::value_ptr(viewProjMat));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(model));
    glUniform4fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(phongMat.ambient));
    glUniform4fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(phongMat.diffuse));
    glUniform4fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(phongMat.specular));
    glUniform4fv(glGetUniformLocation(shader, "emission"), 1, glm::value_ptr(phongMat.emission));
    glUniform1f(glGetUniformLocation(shader, "shininess"), phongMat.shininess);

    // bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

    // unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void PrimitiveMesh::update() {
    return;
}

const glm::mat4& PrimitiveMesh::getModel() const {
    return model;
}

void PrimitiveMesh::spin(float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PrimitiveMesh::moveLocal(glm::vec3 v) {
    model = model * glm::translate(glm::mat4(1), v);
}

void PrimitiveMesh::moveGlobal(glm::vec3 v) {
    model = glm::translate(glm::mat4(1), v) * model;
}

void PrimitiveMesh::generateBuffers() {
    // generate VAO, VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_vertices);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
}

void PrimitiveMesh::bindBuffers() {
    // bind to the VAO.
    glBindVertexArray(VAO);

    // bind to the first VBO, store vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // bind to the second VBO, store normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // generate EBO, bind the EBO to the bound VAO and send data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // unbind VAO and VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

