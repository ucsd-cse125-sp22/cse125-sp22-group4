#pragma once
#ifndef CLIENT_H
#define CLIENT_H

#include "Graphics/include/Cube.h"
#include "Graphics/include/Model.h"
#include "Graphics/include/Camera.h"
#include "Graphics/include/Shader.h"
#include "Graphics/include/ObjectLoader.h"
#include "Graphics/include/ThirdPersonCamera.h"
#include "Graphics/include/Skybox.h"
#include "Graphics/include/SceneLoader.h"

#include "Constants/include/constants.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "spdlog/spdlog.h"

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

    MovementState getMovementState();
    RotationState getRotationState();
    Model** getPlayers();

    void resetRotUpdate();
    void setPlayerfromID(unsigned int id);
};

#endif // !CLIENT_H