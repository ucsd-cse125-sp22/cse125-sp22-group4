#include "Model.h"

static unsigned int TextureFromFile(const char* path,
    const std::string& directory,
    bool gamma = false);

Model::Model(std::string const& path, bool gamma) : gammaCorrection(gamma) {
    loadModel(path);
}

Model::~Model() {
    for (auto mesh : meshes) {
        delete mesh;
    }
}

void Model::draw(const glm::mat4& viewProjMat, GLuint shader) const {
    for (auto& mesh : meshes) {
        mesh->draw(viewProjMat, shader);
    }
}

void Model::update() {
    for (auto& mesh : meshes) {
        mesh->update();
    }
}

void Model::spin(float deg) {
    for (auto& mesh : meshes) {
        mesh->spin(deg);
    }
}

void Model::scale(glm::vec3 rate) {
    for (auto& mesh : meshes) {
        mesh->scale(rate);
    }
}

void Model::moveLocal(glm::vec3 v) {
    for (auto& mesh : meshes) {
        mesh->moveLocal(v);
    }
}

void Model::moveGlobal(glm::vec3 v) {
    for (auto& mesh : meshes) {
        mesh->moveGlobal(v);
    }
}

const glm::mat4& Model::getModel() const {
    return meshes[0]->getModel();
}

void Model::loadModel(std::string const& path) {
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
void Model::processNode(aiNode* node, const aiScene* scene) {
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // recursively process children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

GraphicObject* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector; // placeholder vector to tranfer from assimp vec to glm vec
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates
            // we thus make the assumption that we won't use models where
            // a vertex can have multiple texture coordinates
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces to record indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    /*
    We assume a convention for sampler names in the shaders
    Each diffuse texture should be named as 'texture_diffuseN'
    where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    Same applies to other texture as the following list summarizes:
        diffuse: texture_diffuseN
        specular: texture_specularN
        normal: texture_normalN
    */

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps =
        loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<Texture> specularMaps =
        loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. normal maps
    std::vector<Texture> normalMaps =
        loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. height maps
    std::vector<Texture> heightMaps =
        loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object depending on have textures or not
    if (textures.size() != 0) {
        return new TexturedMesh(vertices, indices, textures);
    }
    else {
        std::vector<glm::vec3> points;
        std::vector<glm::vec3> normals;
        for (auto& vertex : vertices) {
            points.push_back(vertex.Position);
            normals.push_back(vertex.Normal);
        }

        return new PrimitiveMesh(points, normals,indices);
    }
}

/**
 * Checks all material textures of a given type
 * Loads the textures if they're not loaded yet
 *
 * @param   mat         Assimp material
 * @param   type        Type of the texture
 * @param   typeName    Name of the type
 * @return  Vector of textures
**/
std::vector<Texture> Model::loadMaterialTextures(const aiMaterial* mat,
                                                 const aiTextureType type,
                                                 const std::string& typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString texturePath;
        mat->GetTexture(type, i, &texturePath);
        // check if texture was loaded before
        // if so, continue: skip and load new textures only
        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++) {
            // already loaded, skip
            if (std::strcmp(loadedTextures[j].path.data(), texturePath.C_Str()) == 0) {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }
        // if texture hasn't been loaded already, load it
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(texturePath.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = texturePath.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}

unsigned int TextureFromFile(const char* path,
                             const std::string& directory,
                             bool gamma) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        switch (nrComponents) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        spdlog::error("Texture failed to load at: {}", path);
        stbi_image_free(data);
    }

    return textureID;
}
