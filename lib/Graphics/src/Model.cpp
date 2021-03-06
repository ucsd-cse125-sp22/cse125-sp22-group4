#include "Model.h"

Model::Model(std::string const& path, bool gamma) : gammaCorrection(gamma) {
    model = glm::mat4(1);
    maxX = maxZ = -FLT_MAX;
    minX = minZ = FLT_MAX;
    loadModel(path);
}

Model::~Model() {
    for (auto mesh : meshes) {
        delete mesh;
    }
}

void Model::draw(const glm::mat4& viewProjMat, 
                 const glm::mat4& transform,
                 GLuint shader) const {
    for (auto& mesh : meshes) {
        mesh->draw(viewProjMat, transform * model, shader);
    }
}

void Model::update() {
    spin(0.5f);
}

void Model::spin(float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::flip(float deg) {
    model = model * glm::rotate(glm::radians(deg), glm::vec3(1.0f, 0.0f, 0.0f));
}

void Model::rotate(const glm::vec3& v) {
    model = model * glm::rotate(v.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = model * glm::rotate(v.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = model * glm::rotate(v.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Model::scale(const glm::vec3& rate) {
    model = model * glm::scale(rate);
}

void Model::moveLocal(const glm::vec3& v) {
    model = model * glm::translate(glm::mat4(1), v);
}

void Model::moveGlobal(const glm::vec3& v) {
    model = glm::translate(glm::mat4(1), v) * model;
}

void Model::setModel(const glm::mat4& m) {
    model = m;
}

const glm::mat4& Model::getModel() const {
    return model;
}

OBB Model::getOBB() const {
    return CollisionDetector::computeOBB(maxX, maxZ, minX, minZ, model);
}

void Model::loadModel(std::string const& path) {
    spdlog::info("Read file {} via Assimp...", path);
    stbi_set_flip_vertically_on_load(true);
    Assimp::Importer importer;
    const aiScene* scene;

    if (path.substr(path.find_last_of(".") + 1) == "fbx") {
        isFBX = true;
        scene = importer.ReadFile(path, aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace);
    }
    else {
        isFBX = false;
        scene = importer.ReadFile(path, aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices |
            aiProcess_CalcTangentSpace);
    }


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

        SetVertexBoneDataToDefault(vertex);

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
    std::vector<Texture> normalMaps;
    if (isFBX) {
        normalMaps =
            loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }
    else {
        normalMaps =
            loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }

    // 4. AO maps
    std::vector<Texture> ambientOcclusionMaps =
        loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
    textures.insert(textures.end(), ambientOcclusionMaps.begin(), ambientOcclusionMaps.end());

    // 5. Roughness maps
    std::vector<Texture> roughnessMaps =
        loadMaterialTextures(material, aiTextureType_SHININESS, "texture_shininess");
    textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());

    // get traditional Phong material data
    aiColor3D ambient(0);
    aiColor3D diffuse(0);
    aiColor3D specular(0);
    aiColor3D emission(0);
    float shininess = 0;

    material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    material->Get(AI_MATKEY_COLOR_EMISSIVE, emission);
    material->Get(AI_MATKEY_SHININESS, shininess);

    //define the phong material property
    PhongMaterial mat{
    {ambient.r, ambient.g, ambient.b, 1},
    {diffuse.r, diffuse.g, diffuse.b, 1},
    {specular.r, specular.g, specular.b, 1},
    {emission.r, emission.g, emission.b, 1},
    shininess
    };

    ExtractBoneWeightForVertices(vertices, mesh, scene);

    // return a mesh object depending on have textures or not
    if (textures.size() != 0) {
        TexturedMesh* texmesh = new TexturedMesh(vertices, indices, textures, mat);
        if (mesh->mNumBones != 0) {
            texmesh->hasBones = 1;
        }
        OBB obb = texmesh->getOBB();
        maxX = maxX > obb.p1.x ? maxX : obb.p1.x;
        maxZ = maxZ > obb.p1.y ? maxZ : obb.p1.y;
        minX = minX < obb.p3.x ? minX : obb.p3.x;
        minZ = minZ < obb.p3.y ? minZ : obb.p3.y;
        return texmesh;
    }
    else {
        std::vector<glm::vec3> points;
        std::vector<glm::vec3> normals;
        for (auto& vertex : vertices) {
            points.push_back(vertex.Position);
            normals.push_back(vertex.Normal);
        }

        PrimitiveMesh* mesh = new PrimitiveMesh(points, normals, indices, mat);
        OBB obb = mesh->getOBB();
        maxX = maxX > obb.p1.x ? maxX : obb.p1.x;
        maxZ = maxZ > obb.p1.y ? maxZ : obb.p1.y;
        minX = minX < obb.p3.x ? minX : obb.p3.x;
        minZ = minZ < obb.p3.y ? minZ : obb.p3.y;
        return mesh;
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
            std::string filename = std::string(texturePath.C_Str());
            filename = this->directory + '/' + filename;
            texture.id = TextureFromFile(filename);
            texture.type = typeName;
            texture.path = texturePath.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::TextureFromFile(std::string filename, bool gamma) {

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
        spdlog::error("Texture failed to load at: {}", filename);
        stbi_image_free(data);
    }

    return textureID;
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}
void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.m_BoneIDs[i] < 0)
        {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
            break;
        }
    }
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene) {
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end())
        {
            BoneInfo newBoneInfo;
            newBoneInfo.id = m_BoneCounter;
            newBoneInfo.offset = ConvertMatrixToGLMFormat(
                mesh->mBones[boneIndex]->mOffsetMatrix);
            m_BoneInfoMap[boneName] = newBoneInfo;
            boneID = m_BoneCounter;
            m_BoneCounter++;
        }
        else
        {
            boneID = m_BoneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

glm::mat4 Model::ConvertMatrixToGLMFormat(const aiMatrix4x4& from) {
    glm::mat4 to;
    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}


