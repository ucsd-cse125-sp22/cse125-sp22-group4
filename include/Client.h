#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include "Cube.h"
#include "Camera.h"
#include "ThirdPersonCamera.h"
#include "Shader.h"
#include "spdlog/spdlog.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "ObjectLoader.h"

#include <string>

namespace Client {
    GLFWwindow* createWindow(int width, int height, std::string windowTitle);
    void errorCallback(int error, const char* description);
    void setupGLSettings();
    void setupCallbacks(GLFWwindow* window);

    bool initializeClient();
    void displayCallback();
    void idleCallback();
    void cleanup();
    void GUI();
};

#endif // !CLIENT_H