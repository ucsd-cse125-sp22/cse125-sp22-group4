#ifndef FAKE_MODEL_H
#define FAKE_MODEL_H

#include "GraphicObject.h"

#include "stb_image.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <string>
#include <vector>

class FakeModel : public GraphicObject {
private:
    std::string directory;
    glm::mat4 model;
    float maxX, maxZ, minX, minZ;

    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);

public:
    FakeModel() = default;
    FakeModel(const std::string& path);
    ~FakeModel() = default;

    void draw(const glm::mat4& viewProjMat,
              const glm::mat4& transform,
              GLuint shader) const override;
    void update() override;
    void spin(float deg) override;
    void flip(float deg) override;
    void scale(const glm::vec3& rate) override;
    void moveLocal(const glm::vec3& v) override;
    void moveGlobal(const glm::vec3& v) override;
    void setModel(const glm::mat4& m) override;
    const glm::mat4& getModel() const override;
    OBB getOBB() const override;
};

#endif
