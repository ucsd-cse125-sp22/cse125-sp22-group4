#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"
#include <string>

class ObjectLoader : public Mesh {
public:
    ObjectLoader(std::string filename);
    ~ObjectLoader() = default;

    void update() override;
};
#endif // !CUBE_H