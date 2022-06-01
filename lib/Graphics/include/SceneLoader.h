#pragma once

#include "Tokenizer.h"
#include "Model.h"

class SceneLoader{
private:
    std::string scenefilename;
public:
    SceneLoader(std::string filename);
    ~SceneLoader();

    std::vector<Model*> load(const std::string& path);
    void draw(const glm::mat4& viewProjMat,
        const glm::mat4& transform,
        GLuint shader,
        std::vector<Model*> sceneObjects);
};