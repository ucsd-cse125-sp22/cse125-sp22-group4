#include "FakeModel.h"

FakeModel::FakeModel(std::string const& path) {
    model = glm::mat4(1);
    maxX = maxZ = -FLT_MAX;
    minX = minZ = FLT_MAX;
    loadModel(path);
}

void FakeModel::draw(const glm::mat4& viewProjMat, 
                 const glm::mat4& transform,
                 GLuint shader) const {
    spdlog::error("I cannot draw myself! I am fake!");
    return;
}

void FakeModel::update() {
    spin(0.5f);
}

void FakeModel::spin(float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void FakeModel::flip(float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(1.0f, 0.0f, 0.0f));
}

void FakeModel::scale(const glm::vec3& rate) {
    model = model * glm::scale(rate);
}

void FakeModel::moveLocal(const glm::vec3& v) {
    model = model * glm::translate(glm::mat4(1), v);
}

void FakeModel::moveGlobal(const glm::vec3& v) {
    model = glm::translate(glm::mat4(1), v) * model;
}

void FakeModel::setModel(const glm::mat4& m) {
    model = m;
}

const glm::mat4& FakeModel::getModel() const {
    return model;
}

OBB FakeModel::getOBB() const {
    return CollisionDetector::computeOBB(maxX, maxZ, minX, minZ, model);
}

void FakeModel::loadModel(std::string const& path) {
    spdlog::info("Read file {} via Assimp...", path);
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | 
                                                   aiProcess_GenSmoothNormals |
                                                   aiProcess_FlipUVs |
                                                   aiProcess_CalcTangentSpace);
    spdlog::info("Finish reading file.", path);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("Assimp Error: {}.", importer.GetErrorString());
        return;
    }

    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process Assimp's root node recursively
    processNode(scene->mRootNode, scene);
}

/**
 * Processes a node in a recursive fashion
 *
 * @param   node    Current node to process
 * @param   scene   Assimp Scene
**/
void FakeModel::processNode(aiNode* node, const aiScene* scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    // recursively process children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void FakeModel::processMesh(aiMesh* mesh, const aiScene* scene) {
    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        float x = mesh->mVertices[i].x;
        float z = mesh->mVertices[i].z;

        maxX = maxX > x ? maxX : x;
        maxZ = maxZ > z ? maxZ : z;
        minX = minX < x ? minX : x;
        minZ = minZ < z ? minZ : z;
    }
}

