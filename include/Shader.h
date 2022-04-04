#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "spdlog/spdlog.h"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace Shader {
    GLuint loadShaders(const char* vertexFilePath, const char* fragmentFilePath);
};
#endif
