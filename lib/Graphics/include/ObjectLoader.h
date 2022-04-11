#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "PrimitiveMesh.h"
#include <string>

class ObjectLoader : public PrimitiveMesh {
public:
    ObjectLoader(std::string filename);
    ~ObjectLoader() = default;
};
#endif // !CUBE_H