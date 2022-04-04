#include "Cube.h"

Cube::Cube(glm::vec3 cubeMin, glm::vec3 cubeMax) {
    // model matrix.
    model = glm::mat4(1.0f);

    // specify vertex positions
    positions = {
        // front
        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),

        // back
        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),

        // top
        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),

        // bottom
        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),

        // left
        glm::vec3(cubeMin.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMin.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMax.z),
        glm::vec3(cubeMin.x,cubeMax.y,cubeMin.z),

        // right
        glm::vec3(cubeMax.x,cubeMin.y,cubeMax.z),
        glm::vec3(cubeMax.x,cubeMin.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMin.z),
        glm::vec3(cubeMax.x,cubeMax.y,cubeMax.z)
    };

    // specify normals
    normals = {
        // front
        glm::vec3(0,0,1),
        glm::vec3(0,0,1),
        glm::vec3(0,0,1),
        glm::vec3(0,0,1),

        // back
        glm::vec3(0,0,-1),
        glm::vec3(0,0,-1),
        glm::vec3(0,0,-1),
        glm::vec3(0,0,-1),

        // top
        glm::vec3(0,1,0),
        glm::vec3(0,1,0),
        glm::vec3(0,1,0),
        glm::vec3(0,1,0),

        // bottom
        glm::vec3(0,-1,0),
        glm::vec3(0,-1,0),
        glm::vec3(0,-1,0),
        glm::vec3(0,-1,0),

        // left
        glm::vec3(-1,0,0),
        glm::vec3(-1,0,0),
        glm::vec3(-1,0,0),
        glm::vec3(-1,0,0),

        // right
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0),
        glm::vec3(1,0,0)
    };

    // specify indices
    indices = {
        0,1,2,		0,2,3,			// Front
        4,5,6,		4,6,7,			// Back
        8,9,10,		8,10,11,		// Top
        12,13,14,	12,14,15,		// Bottom
        16,17,18,	16,18,19,		// Left
        20,21,22,	20,22,23,		// Right
    };

    generateBuffers();
    bindBuffers();
}

Cube::~Cube() {
    // delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Cube::generateBuffers() {
    // generate VAO, VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
}

void Cube::bindBuffers() {
    // bind to the VAO.
    glBindVertexArray(VAO);

    // bind to the first VBO, store vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
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

void Cube::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // activate the shader program
    glUseProgram(shader);
    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, (float*)&model);

    // bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

    // unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}


void Cube::update() {
    spin(5.0f);
}

void Cube::spin(float deg) {
    // update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Cube::setColor(glm::vec3 c) {
    color = c;
}
