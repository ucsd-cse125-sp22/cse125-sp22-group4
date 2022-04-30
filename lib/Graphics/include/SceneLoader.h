#pragma once

#include "Tokenizer.h"
#include "Model.h"

class SceneLoader{
private:

public:
    SceneLoader(std::string filename);
    ~SceneLoader();

    void load(std::string filename);

    std::vector<Model*> models;
};