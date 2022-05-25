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
#include "Graphics/include/Animator.h"

#include "Constants/include/constants.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "spdlog/spdlog.h"

#include <string>
#include <unordered_set>

namespace Client {
    GLFWwindow* createWindow(int width, int height, std::string windowTitle);
    void errorCallback(int error, const char* description);
    void setupGLSettings();
    void setupCallbacks(GLFWwindow* window);

    bool initializeClient();
    void displayCallback();
    void idleCallback(float dt);
    void cleanup();
    void GUI();
    void timeGUI();
    void ItemHoldGUI();
    void GameOverGUI();
    void miniMapGUI();
    void GameStartGUI();
    void finalDestGUI();
    void stationaryItemGUI();

    MovementState getMovementState();
    RotationState getRotationState();
    Model** getPlayers();

    void resetRotUpdate();
    void setPlayerfromID(unsigned int id);
    void setItemHold(char h);
    void updateItemLocation(glm::mat4 location);
    void updateTime(int t);
    void setCatViewItem(bool c);
    void updateItem2Location(glm::mat4 location);
    void updateItem3Location(glm::mat4 location);
    void setStationaryItemCountdown(float t);
    void setStationaryItem2Countdown(float t);

    bool checkGameStart();
    void setGameStart();
    void setGameOver(int g, int w);


    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
    void calcFinalBoneMatrix(Animator* animator);
    void setNumPlayers(int p);
    void setFinalDest(glm::mat4 location, int f);
};

#endif // !CLIENT_H