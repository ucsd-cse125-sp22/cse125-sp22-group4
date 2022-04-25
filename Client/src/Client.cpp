#include "Client.h"

// shader, camera and light
static GLuint shader;
static GLuint skyboxShader;
static Camera* camera;
static ThirdPersonCamera* thirdPersonCamera;
static int lightCount;
static std::vector<glm::vec4> lightPosn;
static std::vector<glm::vec4> lightColorn;

//skybox
static Skybox* skybox;

// objects
static Cube* ground;
static Model* teapot;
static Model* bunny;
static Model* tyra;
static Model* tyra2;
static Model* player;
static Model* backpack;
static Model* maze;
static Model* players[PLAYER_NUM];

// state variables
unsigned int my_id;
static double prevXPos;
static double prevYPos;
static int select = 0;
static bool pause = false;
static bool showMouse = false;
static bool middlePressed = false;
static bool isThirdPersonCam = false;
static const char* scenes[2] = { "3rd Person Tyra", "Baby Maze" };
static bool keyHeld = false;
static int direction = -1;
static glm::mat4 currRotationUpdate = glm::mat4(1);
static int turn = 0;

// callbacks
static void resizeCallback(GLFWwindow* window, int width, int height);
static void cursorCallback(GLFWwindow* window, double xPos, double yPos);
static void scrollCallback(GLFWwindow* window, double xMove, double yMove);
static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

//todo
enum CamDirections { CAM_FORWARD, CAM_BACK, CAM_LEFT, CAM_RIGHT, CAM_UP };

/**
 * Create a GLFW window of given size
 *
 * @param   width       Window width
 * @param   height      Window height
 * @param   windowTitle Window title
 * @return  Pointer to GLFW window
**/
GLFWwindow* Client::createWindow(int width, int height, std::string windowTitle) {
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    prevXPos = (double) width / 2;
    prevYPos = (double) height / 2;
    return window;
}

/**
 * Print out glfw error
 *
 * @param   error       Error number
 * @param   description Error description
**/
void Client::errorCallback(int error, const char* description) {
    spdlog::error("GLFW Error {}: {}", error, description);
}

/**
 * Setup OpenGL settings like clear color
**/
void Client::setupGLSettings() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_MULTISAMPLE);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

/**
 * Initialize shader program
 *
 * @return  True if success, False otherwise
**/
bool Client::initializeClient() {
    // initialize shader
    //shader = Shader::loadShaders("shaders/basicShader.vert", "shaders/basicShader.frag");
    //shader = Shader::loadShaders("shaders/normalShader.vert", "shaders/normalShader.frag");
    shader = Shader::loadShaders("../../shaders/shader.vert", "../../shaders/shader.frag");
    skyboxShader = Shader::loadShaders("../../shaders/skyboxShader.vert", "../../shaders/skyboxShader.frag");
    if (!shader) {
        spdlog::error("Failed to initialize shader programs.");
        return false;
    }
    if (!skyboxShader) {
        spdlog::error("Failed to initialize skyboxShader programs.");
        return false;
    }

    // initialize camera
    camera = new Camera();

    // initialize light sources
    lightCount = 3;
    lightPosn = { {0, 5, -10, 1}, {0, 5, 10, 1}, {155, 2, -5, 1} };
    lightColorn = { {0.9, 0.6, 0, 1}, {0, 0.6, 0.9, 1}, {0,0.6,0,1} };

    // initialize objects
    ground = new Cube(glm::vec3(-10, -1, -10), glm::vec3(10, 1, 10));
    ground->moveLocal(glm::vec3(0, -3, 0));
    teapot = new Model("../../objects/teapot.obj");
    teapot->scale(glm::vec3(2));
    teapot->moveGlobal(glm::vec3(-5, -2, -5));
    bunny = new Model("../../objects/bunny.obj");
    bunny->scale(glm::vec3(2));
    bunny->moveGlobal(glm::vec3(5, -2, -5));
    tyra = new Model("../../objects/tyra.obj");
    tyra->scale(glm::vec3(1.5));
    tyra->moveGlobal(glm::vec3(0, -0.1, 0));
    //tyra->moveGlobal(glm::vec3(75, 2, -5));
    tyra2 = new Model("../../objects/tyra.obj");
    tyra2->moveGlobal(glm::vec3(2, -0.1, 0));
    //tyra2->moveGlobal(glm::vec3(75, 2, -5));
    backpack = new Model("../../objects/backpack/backpack.obj");
    maze = new Model("../../objects/maze/mazeColorful.obj");
    maze->moveGlobal(glm::vec3(0, 0, 0));
    //tyra->moveLocal(glm::vec3(75, 2, -5));
   
    //hard coded for now
    players[0] = tyra;
    players[1] = teapot;
    players[2] = bunny;
    players[3] = tyra2;

    skybox = new Skybox();

    return true;
}

/**
 * Display objects
**/
void Client::displayCallback() {
    Camera* tempCam;
    if (isThirdPersonCam) {
        tempCam = thirdPersonCamera;
    }
    else {
        tempCam = camera;
    }
    //glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // activate the shader program and send some values
    glUseProgram(shader);
    glUniform3fv(glGetUniformLocation(shader, "eyePos"), 1, glm::value_ptr(tempCam->pos));

    switch (select) {
    case 0:
        glUniform1i(glGetUniformLocation(shader, "lightCount"), lightCount);
        glUniform4fv(glGetUniformLocation(shader, "lightPosn"), lightCount, (float*)lightPosn.data());
        glUniform4fv(glGetUniformLocation(shader, "lightColorn"), lightCount, (float*)lightColorn.data());
        glUseProgram(0);

        for (auto character : players) {
            character->draw(tempCam->viewProjMat, shader);
        }

        ground->draw(tempCam->viewProjMat, shader);

        break;

    case 1:
        //ourModel->draw(tempCam->viewProjMat, shader);
        maze->draw(tempCam->viewProjMat, shader);
        tyra->draw(tempCam->viewProjMat, shader);
        break;
    }

    //draw skybox last for efficiency
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
    //drop right column
    glm::mat4 viewNoTranslate = glm::mat4(glm::mat3(tempCam->view));
    skybox->draw(tempCam->projection* viewNoTranslate, skyboxShader);
}

/**
 * Update objects when idle
**/
void Client::idleCallback() {
    if (isThirdPersonCam) {
        thirdPersonCamera->reset();
        thirdPersonCamera->update();
    }
    else {
        camera->update();
    }
    if (!pause) {
        teapot->update();
        bunny->update();
        backpack->update();
        if (keyHeld == true) {
            switch (direction) {
            //TODO third person movement not networked
            case LEFT:
                if (isThirdPersonCam) {
                    //player->moveLocal(glm::vec3(-0.2, 0, 0));
                    thirdPersonCamera->translateLeft(-0.2f);
                }
                else {
                    //camera->move(glm::vec3(-0.5, 0, 0));
                }
                break;
            case RIGHT:
                if (isThirdPersonCam) {
                    //player->moveLocal(glm::vec3(0.2, 0, 0));
                    thirdPersonCamera->translateRight(-0.2f);
                }
                else {
                    //camera->move(glm::vec3(0.5, 0, 0));
                }
                break;
            case BACK:
                if (isThirdPersonCam) {
                    //player->moveLocal(glm::vec3(0, 0, 0.2));
                    thirdPersonCamera->translateBackward(-0.2f);
                }
                else {
                    //camera->move(glm::vec3(0, 0, 0.5));
                }
                break;
            case FORWARD:
                if (isThirdPersonCam) {
                    //player->moveLocal(glm::vec3(0, 0, -0.2));
                    thirdPersonCamera->translateForward(-0.2f);
                }
                else {
                    //camera->move(glm::vec3(0, 0, -0.5));
                }
            }
      
        }
    }
}

/**
 * Cleanup the client, free all memories
**/
void Client::cleanup() {
    glDeleteProgram(shader);
    glDeleteProgram(skyboxShader);
    delete camera;
    delete thirdPersonCamera;
    delete teapot;
    delete bunny;
    delete tyra;
    delete ground;
    delete backpack;
    delete maze;
    delete skybox;
    
}

/**
 * Client GUI
**/
void Client::GUI() {
    ImGui::Begin("Client GUI");
    ImGui::Checkbox("Pause", &pause);
    ImGui::Checkbox("Third Person Camera", &isThirdPersonCam);
    ImGui::Separator();
    ImGui::Text("Press F to toggle show/hide mouse");
    ImGui::ListBox("Scene Selection", &select, scenes, IM_ARRAYSIZE(scenes), IM_ARRAYSIZE(scenes));
    ImGui::End();
}

/**
 * Setup all the callbacks for GLFW window
 *
 * @param   window  Pointer to GLFW window
**/
void Client::setupCallbacks(GLFWwindow* window) {
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
    glfwSetKeyCallback(window, keyCallback);
}

/**
 * Set viewport and camera correctly for resizing
 *
 * @param   window  Pointer to GLFW window
 * @param   width   New window width
 * @param   height  New window height
**/
static void resizeCallback(GLFWwindow* window, int width, int height) {
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    glViewport(0, 0, width, height);
    thirdPersonCamera->aspectRatio = float(width) / float(height);
}

/**
 * Mouse control
 *
 * @param   window  Pointer to GLFW window
 * @param   xPos    Current x position
 * @param   yPos    Current y position
**/
static void cursorCallback(GLFWwindow* window, double xPos, double yPos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xPos, yPos);
    if (isThirdPersonCam && !pause) {
        if (abs(xPos - prevXPos) > 0.00001 || abs(yPos - prevYPos) > 0.0001) {
            double yawAngle = -0.5 * (xPos - prevXPos);
            double pitchAngle = -0.5 * (yPos - prevYPos);
            turn += (thirdPersonCamera->upVec.y > 0 ? yawAngle : -yawAngle);
            turn = turn % 360;
            //player->spin((float) (thirdPersonCamera->upVec.y > 0 ? yawAngle : -yawAngle));
            float playerSpinDegree = (float)(thirdPersonCamera->upVec.y > 0 ? yawAngle : -yawAngle);
            currRotationUpdate = glm::rotate(glm::radians(playerSpinDegree), glm::vec3(0.0f, 1.0f, 0.0f));
            //thirdPersonCamera->pitch((float)pitchAngle);
          
            prevXPos = xPos;
            prevYPos = yPos;
        }
    } else {
        if (middlePressed) {
            if (abs(xPos - prevXPos) > 0.00001 || abs(yPos - prevYPos) > 0.0001) {
                double yawAngle = -0.5 * (xPos - prevXPos);
                double pitchAngle = -0.5 * (yPos - prevYPos);
                camera->yaw((float)yawAngle);
                camera->pitch((float)pitchAngle);
            }
            prevXPos = xPos;
            prevYPos = yPos;
        }
    }

}

/**
 * Scroll zooming control
 *
 * @param   window  Pointer to GLFW window
 * @param   xMove   X movement
 * @param   yMove   Y movement
**/
static void scrollCallback(GLFWwindow* window, double xMove, double yMove) {
    ImGui_ImplGlfw_ScrollCallback(window, xMove, yMove);
    Camera* tempCam = isThirdPersonCam ? thirdPersonCamera : camera;
    if (yMove > 0) {
        tempCam->zoom(0.9f);
    }
    else if (yMove < 0) {
        tempCam->zoom(1.1f);
    }
}

/**
 * Record mouse button status
 *
 * @param   window  Pointer to GLFW window
 * @param   button  Button ID
 * @param   action  Action ID
 * @param   mods    Mods
**/
static void mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        middlePressed = true;
        glfwGetCursorPos(window, &prevXPos, &prevYPos);
    }
    else {
        middlePressed = false;
    }
}

/**
 * Keyboard control
 *
 * @param   window      Pointer to GLFW window
 * @param   key         Key ID
 * @param   scancode    Scancode
 * @param   action      Action ID
 * @param   mods        Mods
**/
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_SPACE:
            pause = !pause;
            break;

        case GLFW_KEY_R:
            if (isThirdPersonCam) {
                thirdPersonCamera->reset();
            }
            else {
                camera->reset();
            }
            break;

        case GLFW_KEY_W:
            keyHeld = true;
            direction = FORWARD;
            break;

        case GLFW_KEY_S:
            keyHeld = true;
            direction = BACK;
            break;

        case GLFW_KEY_A:
            keyHeld = true;
            direction = LEFT;
            break;

        case GLFW_KEY_D:
            keyHeld = true;
            direction = RIGHT;
            break;

        case GLFW_KEY_F:
            showMouse = !showMouse;
            if (showMouse) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            break;

        case GLFW_KEY_LEFT_SHIFT:
            camera->move(glm::vec3(0, 0.5, 0));
            break;

        case GLFW_KEY_RIGHT_SHIFT:
            camera->move(glm::vec3(0, -0.5, 0));
            break;

        case GLFW_KEY_LEFT_CONTROL:
            camera->move(glm::vec3(-0.5, 0, 0));
            break;

        case GLFW_KEY_RIGHT_CONTROL:
            camera->move(glm::vec3(0.5, 0, 0));
            break;

        case GLFW_KEY_RIGHT:
            camera->move(glm::vec3(0.5, 0, 0));
            break;

        case GLFW_KEY_LEFT:
            camera->move(glm::vec3(-0.5, 0, 0));
            break;

        case GLFW_KEY_UP:
            camera->move(glm::vec3(0, 0, -0.5));
            break;

        case GLFW_KEY_DOWN:
            camera->move(glm::vec3(0, 0, 0.5));
            break;
        }
    }
    else if (action == GLFW_RELEASE)
        keyHeld = false;
}

MovementState Client::getMovementState() {
    return MovementState{
        direction,
        keyHeld
    };
}

RotationState Client::getRotationState() {
    return RotationState{
        currRotationUpdate,
        turn
    };
}

Model** Client::getPlayers() {
    return players;
}

void Client::resetRotUpdate() {
    currRotationUpdate = glm::mat4(1);
}

void Client::updateCam() {
    thirdPersonCamera->updatePos();
}

void Client::setPlayerfromID(unsigned int id) {
    my_id = id;
    player = players[my_id];
    thirdPersonCamera = new ThirdPersonCamera(player);
}