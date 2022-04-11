#ifndef MODEL_H
#define MODEL_H

#include "GraphicObject.h"
#include "TexturedMesh.h"
#include "PrimitiveMesh.h"

#include "stb_image.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "spdlog/spdlog.h"

#include <string>
#include <vector>

class Model : GraphicObject {
private:
	// stores all the textures loaded so far
    // make sure textures aren't loaded more than once.
    std::vector<Texture> loadedTextures;
    std::vector<GraphicObject*> meshes;
    std::string directory;
    bool gammaCorrection;

    void loadModel(std::string const& path);
    void processNode(aiNode* node, const aiScene* scene);
    GraphicObject* processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(const aiMaterial* mat,
        const aiTextureType type,
        const std::string& typeName);

public:
    Model() = default;
    Model(const std::string& path, bool gamma = false);
    ~Model();
    void draw(const glm::mat4& viewProjMat, GLuint shader) const override;
    void update() override;
};

#endif
