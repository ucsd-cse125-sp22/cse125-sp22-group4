#include "TexturedMesh.h"

TexturedMesh::TexturedMesh() {
    VAO = VBO = EBO = 0;
    model = glm::mat4(1);
    phongMat = { glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(0), 0 };
    maxX = maxZ = minX = minZ = 0;
}

TexturedMesh::TexturedMesh(const TexturedMesh& old) {
    vertices = old.vertices;
    indices = old.indices;
    textures = old.textures;
    model = old.model;
    phongMat = old.phongMat;
    maxX = old.maxX;
    maxZ = old.maxZ;
    minX = old.minX;
    minZ = old.minZ;

    generateBuffers();
    bindBuffers();
}

TexturedMesh::TexturedMesh(const std::vector<Vertex>& _vertices,
                           const std::vector<unsigned int>& _indices,
                           const std::vector<Texture>& _textures,
                           const PhongMaterial& _phongMat) {
    vertices = _vertices;
    indices = _indices;
    textures = _textures;
    model = glm::mat4(1);
    phongMat = _phongMat;

    maxX = vertices[0].Position.x;
    maxZ = vertices[0].Position.z;
    minX = maxX;
    minZ = maxZ;
    for (auto& vertex : vertices) {
        maxX = maxX > vertex.Position.x ? maxX : vertex.Position.x;
        maxZ = maxZ > vertex.Position.z ? maxZ : vertex.Position.z;
        minX = minX < vertex.Position.x ? minX : vertex.Position.x;
        minZ = minZ < vertex.Position.z ? minZ : vertex.Position.z;
    }

    generateBuffers();
    bindBuffers();
}

TexturedMesh::~TexturedMesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

// render the mesh
void TexturedMesh::draw(const glm::mat4& viewProjMat, GLuint shader) const {
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, glm::value_ptr(viewProjMat));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(model));
    glUniform1i(glGetUniformLocation(shader, "mode"), 1);
    glUniform4fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(phongMat.ambient));
    glUniform4fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(phongMat.diffuse));
    glUniform4fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(phongMat.specular));
    glUniform4fv(glGetUniformLocation(shader, "emission"), 1, glm::value_ptr(phongMat.emission));
    glUniform1f(glGetUniformLocation(shader, "shininess"), phongMat.shininess);

    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    unsigned int ambientNr = 1;
    unsigned int shininessNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string
        else if (name == "texture_ambient")
            number = std::to_string(ambientNr++); // transfer unsigned int to string
        else if (name == "texture_shininess")
            number = std::to_string(shininessNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // drawOBB mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void TexturedMesh::update() {
    spin(0.5f);
}

void TexturedMesh::spin(float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void TexturedMesh::scale(const glm::vec3& rate) {
    model = model * glm::scale(rate);
}

void TexturedMesh::moveLocal(const glm::vec3& v) {
    model = model * glm::translate(glm::mat4(1), v);
}

void TexturedMesh::moveGlobal(const glm::vec3& v) {
    model = glm::translate(glm::mat4(1), v) * model;
}

void TexturedMesh::setModel(const glm::mat4& m) {
    model = m;
}

const glm::mat4& TexturedMesh::getModel() const {
    return model;
}

OBB TexturedMesh::getOBB() const {
    return CollisionDetector::computeOBB(maxX, maxZ, minX, minZ, model);
}

void TexturedMesh::generateBuffers() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void TexturedMesh::bindBuffers() {
    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    // unbind VAO and VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
